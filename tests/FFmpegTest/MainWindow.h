#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtAV/QtAV.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool openFile(QString fileName);

private slots:
	void on_actionOpen_triggered();

	void on_actionPlay_pause_triggered();

	void on_actionNext_frame_triggered();

	void on_actionPrevious_frame_triggered();

private:
	Ui::MainWindow *ui;

    QtAV::AVPlayer *mpPlayer;
    QtAV::VideoRenderer *mpRenderer;
};

#endif // MAINWINDOW_H
