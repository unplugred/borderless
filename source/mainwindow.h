#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr, QString path = "");
	~MainWindow();

private slots:
	void ShowContextMenu(const QPoint& pos);
	void OpenFile();
	void NextFile();
	void PrevFile();
	void CopyFile();
	void PasteFile();
	void ShowInFolder();
	void ChangeInterpolation();
	void AlwaysOnTop();
	void QuitApp();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	bool LoadImage(QString path = "");
	int compatible(QString path);
	void wheelEvent(QWheelEvent* event);
	void saveSettings();
	void interpolateimg();
	void scaleimg();

	QMenu* contextmenu;
	QAction* openfile;
	QAction* nextfile;
	QAction* prevfile;
	QAction* copyfile;
	QAction* pastefile;
	QAction* showinfolder;
	QAction* nearestneighbor;
	QAction* alwaysontop;
	QAction* quit;

	float scale;
	int width;
	int height;
	QString currentpath = "";
	QString prevpath = "";
	QString nextpath = "";
	QString loopbegin = "";
	bool ontop        = false;
	bool interpolated = true;
	bool initial      = true;
	bool stock        = false;
	bool animated     = false;
	QPixmap img;
	QPoint pressPos;
	QString formats;
	QString movieformats;
	QString dialogpath;

private:
	Ui::MainWindow *ui;
};
#endif
