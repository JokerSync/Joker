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

	// Connect master panel to sony master
	connect(ui->masterPanel, SIGNAL(playButtonSignal()), &_sonyMaster, SLOT(play()));
	connect(ui->masterPanel, SIGNAL(pauseButtonSignal()), &_sonyMaster, SLOT(stop()));
	connect(_sonyMaster.clock(), SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), ui->masterPanel, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_sonyMaster.clock(), SIGNAL(rateChanged(PhRate)), ui->masterPanel, SLOT(onRateChanged(PhRate)));

	// Connect sony master to MainWindow
	connect(ui->queryIdButton, SIGNAL(clicked()), &_sonyMaster, SLOT(deviceTypeRequest()));
	connect(ui->statusSenseButton, SIGNAL(clicked()), &_sonyMaster, SLOT(statusSense()));
	connect(ui->timeSenseButton, SIGNAL(clicked()), &_sonyMaster, SLOT(timeSense()));
	connect(ui->speedSenseButton, SIGNAL(clicked()), &_sonyMaster, SLOT(speedSense()));


	connect(&_sonyMaster, SIGNAL(deviceIdData(unsigned char,unsigned char)), this, SLOT(onDeviceIdData(unsigned char,unsigned char)));
	connect(&_sonyMaster, SIGNAL(statusData(int,unsigned char*)), this, SLOT(onStatusData(int,unsigned char*)));

	// Connect sony slave clock to slave panel
	connect(_sonySlave.clock(), SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), ui->slavePanel, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_sonySlave.clock(), SIGNAL(rateChanged(PhRate)), ui->slavePanel, SLOT(onRateChanged(PhRate)));

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
	connect(&_masterTimer, SIGNAL(timeout()), this, SLOT(tickMaster()));
	connect(&_slaveTimer, SIGNAL(timeout()), this, SLOT(tickSlave()));

//	_masterTimer.start(1000);
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
//	_sonyMaster.statusSense();
}

void MainWindow::tickSlave()
{
	_sonySlave.clock()->tick(25);
}

void MainWindow::onDeviceIdData(unsigned char id1, unsigned char id2)
{
	QString id;
	id.sprintf("%2X %2X", id1, id2);
	ui->idLabel->setText(id);
}

void MainWindow::onStatusData(int length, unsigned char *statusData)
{
	QString statusStr = "";
	for (int i = 0; i < length; i++)
		statusStr += QString::number(statusData[i], 16) + " ";
	ui->statusLabel->setText(statusStr);
}
