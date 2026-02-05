#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMovie>
#include <QMenuBar>
#include <QMimeData>
#include <QFileDialog>
#include <QShortcut>
#include <QClipboard>
#include <QSettings>
#include <QDesktopServices>
#include <QScreen>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent, QString path) : QMainWindow(parent), ui(new Ui::MainWindow) {
	QCoreApplication::setOrganizationName("unplugred");
	QCoreApplication::setOrganizationDomain("https://unplug.red/");
	QCoreApplication::setApplicationName("Borderless");
	QCoreApplication::setApplicationVersion("1.3.0");

	QSettings settings;
	ontop        = settings.value("ontop"       ,false).toBool();
	interpolated = settings.value("interpolated",true ).toBool();

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowIcon(QIcon(":/icons/source/borderless.xpm"));
	setAcceptDrops(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	ui->setupUi(this);
	connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowContextMenu(QPoint)));
	if(ontop)
		setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint|Qt::FramelessWindowHint);
	else
		setWindowFlag(Qt::FramelessWindowHint);

	movieformats = "";
	formats      = "";
	QList<QByteArray> formatlist = QMovie      ::supportedFormats     ();
	foreach(QByteArray format, formatlist) movieformats += "*."+format+" ";
	                  formatlist = QImageReader::supportedImageFormats();
	foreach(QByteArray format, formatlist) formats      += "*."+format+" ";

	QShortcut *leftshortcut  = new QShortcut(Qt::Key_Left       ,this);
	QShortcut *rightshortcut = new QShortcut(Qt::Key_Right      ,this);
	QShortcut *copyshortcut  = new QShortcut(QKeySequence::Copy ,this);
	QShortcut *pasteshortcut = new QShortcut(QKeySequence::Paste,this);
	QShortcut *escshortcut   = new QShortcut(Qt::Key_Escape     ,this);
	connect(leftshortcut ,SIGNAL(activated()),this,SLOT(NextFile()));
	connect(rightshortcut,SIGNAL(activated()),this,SLOT(PrevFile()));
	connect(copyshortcut ,SIGNAL(activated()),this,SLOT(CopyFile()));
	connect(pasteshortcut,SIGNAL(activated()),this,SLOT(PasteFile()));
	connect(escshortcut  ,SIGNAL(activated()),this,SLOT(QuitApp()));

	openfile        = new QAction(tr("Open"            ),this);
	nextfile        = new QAction(tr("Next"            ),this);
	prevfile        = new QAction(tr("Previous"        ),this);
	copyfile        = new QAction(tr("Copy"            ),this);
	pastefile       = new QAction(tr("Paste"           ),this);
	showinfolder    = new QAction(tr("Open folder"     ),this);
	nearestneighbor = new QAction(tr("Nearest neighbor"),this);
	alwaysontop     = new QAction(tr("Always on top"   ),this);
	quit            = new QAction(tr("Quit"            ),this);
	nextfile ->setShortcut(Qt::Key_Left       );
	prevfile ->setShortcut(Qt::Key_Right      );
	copyfile ->setShortcut(QKeySequence::Copy );
	pastefile->setShortcut(QKeySequence::Paste);
	quit     ->setShortcut(Qt::Key_Escape     );
	connect(openfile       ,SIGNAL(triggered()),this,SLOT(OpenFile           ()));
	connect(nextfile       ,SIGNAL(triggered()),this,SLOT(NextFile           ()));
	connect(prevfile       ,SIGNAL(triggered()),this,SLOT(PrevFile           ()));
	connect(copyfile       ,SIGNAL(triggered()),this,SLOT(CopyFile           ()));
	connect(pastefile      ,SIGNAL(triggered()),this,SLOT(PasteFile          ()));
	connect(showinfolder   ,SIGNAL(triggered()),this,SLOT(ShowInFolder       ()));
	connect(nearestneighbor,SIGNAL(triggered()),this,SLOT(ChangeInterpolation()));
	connect(alwaysontop    ,SIGNAL(triggered()),this,SLOT(AlwaysOnTop        ()));
	connect(quit           ,SIGNAL(triggered()),this,SLOT(QuitApp            ()));
	nearestneighbor->setCheckable(true);
	alwaysontop    ->setCheckable(true);
	nearestneighbor->setChecked(!interpolated);
	alwaysontop    ->setChecked(ontop);

	contextmenu = new QMenu(tr("Context menu"),this);
	contextmenu->addAction(openfile       );
	contextmenu->addAction(nextfile       );
	contextmenu->addAction(prevfile       );
	contextmenu->addSeparator();
	contextmenu->addAction(copyfile       );
	contextmenu->addAction(pastefile      );
	contextmenu->addAction(showinfolder   );
	contextmenu->addSeparator();
	contextmenu->addAction(nearestneighbor);
	contextmenu->addAction(alwaysontop    );
	contextmenu->addSeparator();
	contextmenu->addAction(quit           );

	LoadImage(path);
}
MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::LoadImage(QString path, bool loadnext) {
	if(path.isEmpty()) {
		path = ":/icons/source/borderless.xpm";
		if(!stock) {
			copyfile       ->setEnabled(false);
			showinfolder   ->setEnabled(false);
			nearestneighbor->setEnabled(false);
			nearestneighbor->setChecked(true);
			stock = true;
		}
	} else if(stock) {
		copyfile       ->setEnabled(true);
		showinfolder   ->setEnabled(true);
		nearestneighbor->setEnabled(true);
		nearestneighbor->setChecked(!interpolated);
		stock = false;
	}
	path = QFileInfo(path).canonicalFilePath();
	QString nameoffile = QFileInfo(path).fileName();

	QDir dir = QFileInfo(path).dir();
	QStringList files = dir.entryList();
	bool found = false;
	bool seekinglast = false;
	nextpath = "";
	prevpath = "";
	currentpath = path;
	if(!stock) {
		foreach(QString filename, files) {
			if(filename == nameoffile) {
				found = true;
			} else if(compatible(filename) > 0) {
				if(found) {
					if(seekinglast) {
						prevpath = dir.absoluteFilePath(filename);
					} else {
						nextpath = dir.absoluteFilePath(filename);
						if(!prevpath.isEmpty()) {
							break;
						} else {
							prevpath = dir.absoluteFilePath(filename);
							seekinglast = true;
						}
					}
				} else {
					prevpath = dir.absoluteFilePath(filename);
					if(nextpath.isEmpty()) nextpath = dir.absoluteFilePath(filename);
				}
			}
		}
	}
	if(nextpath.isEmpty()) {
		nextfile->setEnabled(false);
		prevfile->setEnabled(false);
	} else {
		nextfile->setEnabled(true);
		prevfile->setEnabled(true);
	}

	if(QFile::exists(path)) {
		setWindowTitle(stock?"Borderless":nameoffile);

		animated = compatible(path) == 2;
		if(animated) {
			QMovie *movie = new QMovie(path);
			ui->img->setMovie(movie);
			movie->start();
			width  = movie->frameRect().width ();
			height = movie->frameRect().height();
			scaleimg();
		} else {
			img = QPixmap(path);
			width  = img.width ();
			height = img.height();
			scaleimg();
			interpolateimg();
		}
	} else {
		path = loadnext?nextpath:prevpath;
		if(QFile::exists(path)) {
			LoadImage(path,loadnext);
		} else {
			nextfile    ->setEnabled(false);
			prevfile    ->setEnabled(false);
			showinfolder->setEnabled(false);
		}
	}
}

void MainWindow::scaleimg() {
	scale = stock?5:1;

	QPoint center;
	QRect screen;
	if(initial) {
		center = QCursor::pos();
		screen = qApp->screenAt(center)->availableGeometry();
		initial = false;
	} else {
		center = geometry().center();
		screen = qApp->screenAt(geometry().center())->availableGeometry();
	}

	if(width        < 32) scale = 32.0f/width ;
	if(height*scale < 32) scale = 32.0f/height;
	if(width *scale > screen.width ()) scale = (float)screen.width ()/width ;
	if(height*scale > screen.height()) scale = (float)screen.height()/height;
	int scaledwidth  = qRound(width *scale);
	int scaledheight = qRound(height*scale);
	setGeometry(
		std::min(std::max(screen.left(),qRound((center.x()-scaledwidth *.5+1)*.5)*2)+scaledwidth ,screen.right ())-scaledwidth ,
		std::min(std::max(screen.top (),qRound((center.y()-scaledheight*.5+1)*.5)*2)+scaledheight,screen.bottom())-scaledheight,
		scaledwidth ,
		scaledheight);
	ui->img->setGeometry(0,0,
		scaledwidth *.05f,
		scaledheight*.05f);
}

void MainWindow::interpolateimg() {
	if(animated) {
		ui->img->movie()->setScaledSize((interpolated&&!stock)?QSize(qRound(width*scale),qRound(height*scale)):QSize(width,height));
	} else {
		if(scale == 1)
			ui->img->setPixmap(img);
		else
			ui->img->setPixmap(img.scaled(qRound(width*scale),qRound(height*scale),Qt::IgnoreAspectRatio,(interpolated&&!stock)?Qt::SmoothTransformation:Qt::FastTransformation));
	}
}

void MainWindow::wheelEvent(QWheelEvent* event) {
	QPointF anchor = QPointF(
		(event->globalPosition().x()-geometry().left())/(width *scale),
		(event->globalPosition().y()-geometry().top ())/(height*scale));
	scale += (event->angleDelta().y()*.2)/width;
	if(width *scale < 32) scale = 32.0f/width ;
	if(height*scale < 32) scale = 32.0f/height;
	int scaledwidth  = qRound(width *scale);
	int scaledheight = qRound(height*scale);
	setGeometry(
		event->globalPosition().x()-scaledwidth *anchor.x(),
		event->globalPosition().y()-scaledheight*anchor.y(),
		scaledwidth ,
		scaledheight);
	ui->img->setGeometry(0,0,
		scaledwidth ,
		scaledheight);
	interpolateimg();
}

void MainWindow::ShowContextMenu(const QPoint &pos) {
	contextmenu->exec(mapToGlobal(pos));
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
	pressPos = event->globalPosition().toPoint()-geometry().topLeft();
	event->accept();
}
void MainWindow::mouseMoveEvent(QMouseEvent* event) {
	move(event->globalPosition().x()-pressPos.x(),event->globalPosition().y()-pressPos.y()); // TODO works when commented out ???????
	event->accept();
}

int MainWindow::compatible(QString path) {
	if(movieformats.contains("*"+path.toLower().right(path.length()-path.lastIndexOf("."))+" "))
		return 2;
	else if(formats.contains("*"+path.toLower().right(path.length()-path.lastIndexOf("."))+" "))
		return 1;
	else
		return 0;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	const QMimeData* mimeData = event->mimeData();
	if(mimeData->hasUrls()) {
		QString path = mimeData->urls().at(0).toLocalFile();
		if(compatible(path) > 0) event->acceptProposedAction();
	}
}
void MainWindow::dropEvent(QDropEvent *event) {
	LoadImage(event->mimeData()->urls().at(0).toLocalFile());
	event->acceptProposedAction();
}

void MainWindow::OpenFile() {
	QString path = QFileDialog::getOpenFileName(this,tr("Open image"),stock?QDir::homePath():QFileInfo(currentpath).absolutePath(),tr("Image files ("+formats.left(formats.length()-1).toLocal8Bit()+");;All files (*.*)"));
	if(!path    .isNull ()) LoadImage(path);
}
void MainWindow::NextFile() {
	if(!nextpath.isEmpty()) LoadImage(nextpath);
}
void MainWindow::PrevFile() {
	if(!prevpath.isEmpty()) LoadImage(prevpath,false);
}
void MainWindow::CopyFile() {
	QApplication::clipboard()->setImage(QImage(currentpath),QClipboard::Clipboard);
}
void MainWindow::PasteFile() {
	if(!QApplication::clipboard()->mimeData()->hasImage()) return;

	setWindowTitle("Borderless");

	img = qvariant_cast<QPixmap>(QApplication::clipboard()->mimeData()->imageData());
	width  = img.width ();
	height = img.height();

	scaleimg();
	interpolateimg();

	nextpath    = "";
	prevpath    = "";
	currentpath = "";
	nextfile    ->setEnabled(false);
	prevfile    ->setEnabled(false);
	showinfolder->setEnabled(false);

	show();
}
void MainWindow::ShowInFolder() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(currentpath).absolutePath()));
}
void MainWindow::ChangeInterpolation() {
	interpolated = !interpolated;
	interpolateimg();
	saveSettings();
}
void MainWindow::AlwaysOnTop() {
	ontop = !ontop;
	saveSettings();
	if(ontop)
		setWindowFlags( windowFlags()| Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint  |Qt::FramelessWindowHint);
	else
		setWindowFlags((windowFlags()^(Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint))|Qt::FramelessWindowHint);
	show();
}
void MainWindow::QuitApp() {
	QCoreApplication::exit();
}

void MainWindow::saveSettings() {
	QSettings settings;
	settings.setValue("version",QApplication::applicationVersion());
	settings.setValue("ontop",ontop);
	settings.setValue("interpolated",interpolated);
	settings.sync();
}
