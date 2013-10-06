#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <PhGraphicStrip/PhGraphicStripView.h>
#include <PhVideo/PhVideoView.h>
#include <QMessageBox>
#include <QSettings>

#include "VideoStripSynchronizer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void openFile(QString fileName);

	bool openVideoFile(QString videoFileName);

private slots:
	void onOpenFile();

	void on_actionPlay_pause_triggered();

	void on_actionPlay_backward_triggered();

	void on_actionStep_forward_triggered();

	void on_actionStep_backward_triggered();

	void on_actionStep_time_forward_triggered();

	void on_actionStep_time_backward_triggered();

	void on_action_3_triggered();

	void on_action_1_triggered();

	void on_action_0_5_triggered();

	void on_action0_triggered();

	void on_action0_5_triggered();

	void on_action1_triggered();

	void on_action3_triggered();

	void on_actionGo_To_triggered();

	void on_actionOpen_Video_triggered();

	void on_actionSet_Time_Code_triggered();

	void on_actionChange_font_triggered();

private:
	Ui::MainWindow *ui;
	PhGraphicStripView *_stripView;
	PhStripDoc *_doc;
	PhClock *_stripClock;
	SonyVideoStripSynchronizer _stripVideoSynchronizer;
	QSettings _settings;
};

#endif // MAINWINDOW_H
