#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
	void tickMaster();
	void tickSlave();

	void on_queryIdButton_clicked();

	void onDeviceIdAnswer(unsigned char id1, unsigned char id2);

private:
	Ui::MainWindow *ui;
	PhSonyMasterController _sonyMaster;
	PhSonySlaveController _sonySlave;
	QTimer _slaveTimer, _masterTimer;
};

#endif // MAINWINDOW_H
