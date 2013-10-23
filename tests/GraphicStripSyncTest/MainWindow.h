#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhSync/PhClockSynchronizer.h"
#include "PhSync/PhSonySlaveController.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QSettings *settings, QWidget *parent = 0);
	~MainWindow();

	void openFile(QString fileName);

private slots:
	void onOpenFile();
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);

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

private:
	Ui::MainWindow *ui;
	PhGraphicStrip *_strip;
	PhStripDoc *_doc;
	PhClock *_clock;
	QSettings * _settings;
	PhSonySlaveController _sonySlave;
	PhClockSynchronizer _clockSynchroniser;
};

#endif // MAINWINDOW_H
