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
	QCoreApplication::setApplicationName("borderless");
	QCoreApplication::setApplicationVersion("1.3.0");

	QSettings settings;
	ontop = settings.value("ontop",false).toBool();
	interpolated = settings.value("interpolated",true).toBool();

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowIcon(QIcon(":/icons/borderless.xpm"));
	setAcceptDrops(true);
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->setupUi(this);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));
	if(ontop) setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
	else setWindowFlag(Qt::FramelessWindowHint);

	QList<QByteArray> formatlist = QMovie::supportedFormats();
	movieformats = "";
	foreach(QByteArray format, formatlist) movieformats += "*." + format + " ";
	formatlist = QImageReader::supportedImageFormats();
	formats = "";
	foreach(QByteArray format, formatlist) formats += "*." + format + " ";

	QShortcut *leftshortcut = new QShortcut(Qt::Key_Left, this);
	connect(leftshortcut,SIGNAL(activated()),this,SLOT(NextFile()));
	QShortcut *rightshortcut = new QShortcut(Qt::Key_Right, this);
	connect(rightshortcut,SIGNAL(activated()),this,SLOT(PrevFile()));
	QShortcut *copyshortcut = new QShortcut(QKeySequence::Copy, this);
	connect(copyshortcut,SIGNAL(activated()),this,SLOT(CopyFile()));
	QShortcut *pasteshortcut = new QShortcut(QKeySequence::Paste, this);
	connect(pasteshortcut,SIGNAL(activated()),this,SLOT(PasteFile()));
	QShortcut *escshortcut = new QShortcut(Qt::Key_Escape, this);
	connect(escshortcut,SIGNAL(activated()),this,SLOT(QuitApp()));

	contextmenu = new QMenu(tr("Context menu"), this);

	openfile = new QAction(tr("Open"), this);
	connect(openfile, SIGNAL(triggered()), this, SLOT(OpenFile()));
	contextmenu->addAction(openfile);

	nextfile = new QAction(tr("Next"), this);
	nextfile->setShortcut(Qt::Key_Left);
	connect(nextfile, SIGNAL(triggered()), this, SLOT(NextFile()));
	contextmenu->addAction(nextfile);

	prevfile = new QAction(tr("Previous"), this);
	prevfile->setShortcut(Qt::Key_Right);
	connect(prevfile, SIGNAL(triggered()), this, SLOT(PrevFile()));
	contextmenu->addAction(prevfile);

	contextmenu->addSeparator();

	copyfile = new QAction(tr("Copy"), this);
	copyfile->setShortcut(QKeySequence::Copy);
	connect(copyfile, SIGNAL(triggered()), this, SLOT(CopyFile()));
	contextmenu->addAction(copyfile);

	pastefile = new QAction(tr("Paste"), this);
	pastefile->setShortcut(QKeySequence::Paste);
	connect(pastefile, SIGNAL(triggered()), this, SLOT(PasteFile()));
	contextmenu->addAction(pastefile);

	showinfolder = new QAction(tr("Open folder"), this);
	connect(showinfolder, SIGNAL(triggered()), this, SLOT(ShowInFolder()));
	contextmenu->addAction(showinfolder);

	contextmenu->addSeparator();

	nearestneighbor = new QAction(tr("Nearest neighbor"), this);
	nearestneighbor->setCheckable(true);
	nearestneighbor->setChecked(!interpolated);
	connect(nearestneighbor, SIGNAL(triggered()), this, SLOT(ChangeInterpolation()));
	contextmenu->addAction(nearestneighbor);

	alwaysontop = new QAction(tr("Always on top"), this);
	alwaysontop->setCheckable(true);
	alwaysontop->setChecked(ontop);
	connect(alwaysontop, SIGNAL(triggered()), this, SLOT(AlwaysOnTop()));
	contextmenu->addAction(alwaysontop);

	contextmenu->addSeparator();

	quit = new QAction(tr("Quit"), this);
	quit->setShortcut(Qt::Key_Escape);
	connect(quit, SIGNAL(triggered()), this, SLOT(QuitApp()));
	contextmenu->addAction(quit);

	LoadImage(path);
}
MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::LoadImage(QString path, bool loadnext) {
	if(path.isEmpty()) {
		path = ":/icons/borderless.xpm";
		if(!stock) {
			copyfile->setEnabled(false);
			showinfolder->setEnabled(false);
			nearestneighbor->setEnabled(false);
			nearestneighbor->setChecked(true);
			stock = true;
		}
	} else if(stock) {
		copyfile->setEnabled(true);
		showinfolder->setEnabled(true);
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
					if(seekinglast) prevpath =  dir.absoluteFilePath(filename);
					else {
						nextpath = dir.absoluteFilePath(filename);
						if(!prevpath.isEmpty()) break;
						else {
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
		setWindowTitle(stock ? "Borderless" : nameoffile);

		animated = compatible(path) == 2;
		if(animated) {
			QMovie *movie = new QMovie(path);
			ui->label->setMovie(movie);
			movie->start();
			width = movie->frameRect().width();
			height = movie->frameRect().height();
			scaleimg();
		} else {
			img = QPixmap(path);
			width = img.width();
			height = img.height();
			scaleimg();
			interpolateimg();
		}
	} else {
		path = loadnext ? nextpath : prevpath;
		if(QFile::exists(path)) {
			LoadImage(path,loadnext);
		} else {
			nextfile->setEnabled(false);
			prevfile->setEnabled(false);
			showinfolder->setEnabled(false);
		}
	}
}

void MainWindow::scaleimg() {
	scale = stock ? 5 : 1;

	QPoint center;
	QRect screen;
	if(initial) {
		center = QCursor::pos();
		screen = qApp->screenAt(center)->availableGeometry();
		initial = false;
	} else {
		center = this->geometry().center();
		screen = qApp->screenAt(this->geometry().center())->availableGeometry();
	}

	if(width < 32) scale = 32.0f/width;
	if(height*scale < 32) scale = 32.0f/height;
	if(width*scale > screen.width()) scale = (float)screen.width() / width;
	if(height*scale > screen.height()) scale = (float)screen.height() / height;
	this->setGeometry(
		std::min((int)(std::max(screen.left(),(int)(qRound((center.x()-width*scale*.5+1)*.5)*2))+width*scale),screen.right())-width*scale,
		std::min((int)(std::max(screen.top(),(int)(qRound((center.y()-height*scale*.5+1)*.5)*2))+height*scale),screen.bottom())-height*scale,
		width*scale,
		height*scale);
	ui->label->setGeometry(0,0,width*scale,height*scale);
}

void MainWindow::interpolateimg() {
	if(animated) {
		ui->label->movie()->setScaledSize((interpolated && !stock) ? QSize(width*scale,height*scale) : QSize(width,height));
	} else {
		if(scale == 1) ui->label->setPixmap(img);
		else ui->label->setPixmap(img.scaled(width*scale,height*scale, Qt::IgnoreAspectRatio, (interpolated && !stock) ? Qt::SmoothTransformation : Qt::FastTransformation));
	}
}

void MainWindow::wheelEvent(QWheelEvent* event) {
	QPoint center = this->geometry().center();
	scale += (event->angleDelta().y()*.2)/width;
	if(width*scale < 32) scale = 32.0f/width;
	if(height*scale < 32) scale = 32.0f/height;
	this->setGeometry(
		qRound((center.x()-width*scale*.5+1)*.5)*2,
		qRound((center.y()-height*scale*.5+1)*.5)*2,
		width*scale,
		height*scale);
	ui->label->setGeometry(0,0,width*scale,height*scale);
	interpolateimg();
}

void MainWindow::ShowContextMenu(const QPoint &pos) {
	contextmenu->exec(mapToGlobal(pos));
}

void MainWindow::mousePressEvent(QMouseEvent* event){
	pressPos = event->globalPos()-geometry().topLeft();
	event->accept();
}
void MainWindow::mouseMoveEvent(QMouseEvent* event){
	window()->move(event->globalPos() - pressPos);
	event->accept();
}

int MainWindow::compatible(QString path) {
	if(movieformats.contains("*" + path.toLower().right(path.length() - path.lastIndexOf(".")) + " "))
		return 2;
	else if(formats.contains("*" + path.toLower().right(path.length() - path.lastIndexOf(".")) + " "))
		return 1;
	else
		return 0;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	const QMimeData* mimeData = event->mimeData();
	if(mimeData->hasUrls()) {
		QString path = mimeData->urls().at(0).toLocalFile();
		if(compatible(path) > 0) event->acceptProposedAction();
	}
}
void MainWindow::dropEvent(QDropEvent *event)
{
	LoadImage(event->mimeData()->urls().at(0).toLocalFile());
	event->acceptProposedAction();
}

void MainWindow::OpenFile()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open image"), stock ? QDir::homePath() : QFileInfo(currentpath).absolutePath(), tr("Image files (" + formats.left(formats.length()-1).toLocal8Bit() + ");;All files (*.*)"));
	if(!path.isNull()) LoadImage(path);
}
void MainWindow::NextFile()
{
	if(!nextpath.isEmpty()) LoadImage(nextpath);
}
void MainWindow::PrevFile()
{
	if(!prevpath.isEmpty()) LoadImage(prevpath, false);
}
void MainWindow::CopyFile()
{
	QApplication::clipboard()->setImage(QImage(currentpath),QClipboard::Clipboard);
}
void MainWindow::PasteFile()
{
	if(!QApplication::clipboard()->mimeData()->hasImage()) return;

	setWindowTitle("Borderless");

	img = qvariant_cast<QPixmap>(QApplication::clipboard()->mimeData()->imageData());
	width = img.width();
	height = img.height();

	scaleimg();
	interpolateimg();

	nextpath = "";
	prevpath = "";
	currentpath = "";
	nextfile->setEnabled(false);
	prevfile->setEnabled(false);
	showinfolder->setEnabled(false);

	show();
}
void MainWindow::ShowInFolder()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(currentpath).absolutePath()));
}
void MainWindow::ChangeInterpolation()
{
	interpolated = !interpolated;
	interpolateimg();
	saveSettings();
}
void MainWindow::AlwaysOnTop()
{
	ontop = !ontop;
	saveSettings();
	if(ontop) setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
	else setWindowFlags((windowFlags() ^ (Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint)) | Qt::FramelessWindowHint);
	show();
}
void MainWindow::QuitApp()
{
	QCoreApplication::exit();
}

void MainWindow::saveSettings()
{
	QSettings settings;
	settings.setValue("version",QApplication::applicationVersion());
	settings.setValue("ontop",ontop);
	settings.setValue("interpolated",interpolated);
	settings.sync();
}
