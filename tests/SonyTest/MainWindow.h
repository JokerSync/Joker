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
	void tickMaster();
	void tickSlave();

	void onDeviceIdData(unsigned char id1, unsigned char id2);
	void onStatusData(int length, unsigned char * statusData);
	void on_masterActiveCheck_clicked(bool checked);
	void on_slaveActiveCheck_clicked(bool checked);


private:
	Ui::MainWindow *ui;
	PhSonyMasterController _sonyMaster;
	PhSonySlaveController _sonySlave;
	QTimer _slaveTimer, _masterTimer;
};

#endif // MAINWINDOW_H
