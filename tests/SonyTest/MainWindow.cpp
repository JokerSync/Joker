#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_sonyMaster(this),
	_sonySlave(this)
{
	ui->setupUi(this);

	// configure panels
	ui->masterPanel->setMediaLength(1000);

	ui->slavePanel->setMediaLength(10000);
	ui->slavePanel->setClock(&_slaveClock);

//	connect(ui->masterPanel, SIGNAL(playButtonSignal()), &_sonyMaster, SLOT(play()));
//	connect(ui->masterPanel, SIGNAL(pauseButtonSignal()), &_sonyMaster, SLOT(pause()));

	// start slave and master
	if(_sonyMaster.start())
	{
		qDebug() << "master open ok";

		if(_sonySlave.start())
		{
			qDebug() << "slave open ok";
			_sonyMaster.deviceTypeRequest();
		}
		else
			qDebug() << "error opening master";
	}
	else
		qDebug() << "error opening master";

	// start timers
//	connect(&_masterTimer, SIGNAL(timeout()), this, SLOT(tickMaster()));
	connect(&_slaveTimer, SIGNAL(timeout()), this, SLOT(tickSlave()));

//	_masterTimer.start(7000);
	_slaveTimer.start(40);

//	_sonySlave.getClock()->setRate(1);
}

MainWindow::~MainWindow()
{
	_sonyMaster.stop();
	_sonySlave.stop();
	delete ui;
}

void MainWindow::tickMaster()
{
	_sonyMaster.timeSense();
	_sonyMaster.statusSense();
}

void MainWindow::tickSlave()
{
	_slaveClock.tick(25);
}
