#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "PhClock.h"
#include "PhSonyMasterController.h"
#include "PhSonySlaveController.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void masterPlayPause();
	void masterNextFrame();
	void masterPreviousFrame();

	void onDeviceIdData(unsigned char id1, unsigned char id2);
	void onStatusData(unsigned char * statusData, int offset, int length);
	void on_masterActiveCheck_clicked(bool checked);
	void on_slaveActiveCheck_clicked(bool checked);


	void on_actionMaster_GoTo_triggered();

	void on_actionSlave_GoTo_triggered();

private:
	Ui::MainWindow *ui;
	PhSonyMasterController _sonyMaster;
	PhSonySlaveController _sonySlave;
	QTimer _videosyncCheckTimer;
};

#endif // MAINWINDOW_H
