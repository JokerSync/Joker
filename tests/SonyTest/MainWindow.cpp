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
	ui->masterPanel->setClock(&_masterClock);

	ui->slavePanel->setMediaLength(10000);
	ui->slavePanel->setClock(&_slaveClock);

	connect(ui->masterPanel, SIGNAL(playButtonSignal()), &_sonyMaster, SLOT(play()));
	connect(ui->masterPanel, SIGNAL(pauseButtonSignal()), &_sonyMaster, SLOT(stop()));
	connect(&_sonySlave, SIGNAL(rateChanged(PhRate)), &_slaveClock, SLOT(setRate(PhRate)));

	connect(&_sonyMaster, SIGNAL(deviceIdAnswer(unsigned char,unsigned char)), this, SLOT(onDeviceIdAnswer(unsigned char,unsigned char)));

	// start slave and master
	if(_sonyMaster.open())
	{
		qDebug() << "master open ok";

		if(_sonySlave.open())
			qDebug() << "slave open ok";
		else
			qDebug() << "error opening master";
	}
	else
		qDebug() << "error opening master";

	// start timers
//	connect(&_masterTimer, SIGNAL(timeout()), this, SLOT(tickMaster()));
	connect(&_slaveTimer, SIGNAL(timeout()), this, SLOT(tickSlave()));

//	_masterTimer.start(40);
	_slaveTimer.start(40);

//	_sonySlave.getClock()->setRate(1);
}

MainWindow::~MainWindow()
{
	_sonyMaster.close();
	_sonySlave.close();
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

void MainWindow::on_queryIdButton_clicked()
{
	_sonyMaster.deviceTypeRequest();
}

void MainWindow::onDeviceIdAnswer(unsigned char id1, unsigned char id2)
{
	QString id;
	id.sprintf("%2X %2X", id1, id2);
	ui->idLabel->setText(id);
}
