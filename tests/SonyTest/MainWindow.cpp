#include <QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "PhTools/PhDebug.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_sonyMaster(PhTimeCodeType25, this),
	_sonySlave(PhTimeCodeType25, this)
{
	ui->setupUi(this);

	// configure panels
	ui->masterPanel->setMediaLength(10000);

	ui->slavePanel->setMediaLength(10000);

	// Connect master panel to sony master
	connect(ui->masterPanel, SIGNAL(playPause()), this, SLOT(masterPlayPause()));
	connect(ui->masterPanel, SIGNAL(nextFrame()), this, SLOT(masterNextFrame()));
	connect(ui->masterPanel, SIGNAL(previousFrame()), this, SLOT(masterPreviousFrame()));
	connect(ui->masterPanel, SIGNAL(fastForward()), &_sonyMaster, SLOT(fastForward()));
	connect(ui->masterPanel, SIGNAL(rewind()), &_sonyMaster, SLOT(rewind()));

	connect(_sonyMaster.clock(), SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), ui->masterPanel, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_sonyMaster.clock(), SIGNAL(rateChanged(PhRate)), ui->masterPanel, SLOT(onRateChanged(PhRate)));

	// Connect sony master to MainWindow
	connect(ui->queryIdButton, SIGNAL(clicked()), &_sonyMaster, SLOT(deviceTypeRequest()));
	connect(ui->statusSenseButton, SIGNAL(clicked()), &_sonyMaster, SLOT(statusSense()));
	connect(ui->timeSenseButton, SIGNAL(clicked()), &_sonyMaster, SLOT(timeSense()));
	connect(ui->speedSenseButton, SIGNAL(clicked()), &_sonyMaster, SLOT(speedSense()));

	connect(&_sonyMaster, SIGNAL(deviceIdData(unsigned char,unsigned char)), this, SLOT(onDeviceIdData(unsigned char,unsigned char)));
	connect(&_sonyMaster, SIGNAL(statusData(unsigned char*, int, int)), this, SLOT(onStatusData(unsigned char*, int, int)));

	// Connect sony slave clock to slave panel
	connect(_sonySlave.clock(), SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), ui->slavePanel, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_sonySlave.clock(), SIGNAL(rateChanged(PhRate)), ui->slavePanel, SLOT(onRateChanged(PhRate)));

	// Connect video sync signal
	connect(&_sonyMaster, SIGNAL(videoSync()), &_sonyMaster, SLOT(onVideoSync()));
	connect(&_sonySlave, SIGNAL(videoSync()), &_sonySlave, SLOT(onVideoSync()));
	// start master and slave
	on_masterActiveCheck_clicked(true);
	on_slaveActiveCheck_clicked(true);

	// start timers
	connect(&_videosyncCheckTimer, SIGNAL(timeout()), &_sonyMaster, SLOT(checkVideoSync()));
	connect(&_videosyncCheckTimer, SIGNAL(timeout()), &_sonySlave, SLOT(checkVideoSync()));

//	_masterTimer.start(1000);
	_videosyncCheckTimer.start(5);
	_sonySlave.clock()->setFrame(25 * 25);

//	_sonySlave.getClock()->setRate(1);
}

MainWindow::~MainWindow()
{
	_sonyMaster.close();
	_sonySlave.close();
	delete ui;
}

void MainWindow::masterPlayPause()
{
	if(_sonyMaster.clock()->rate() != 0)
		_sonyMaster.stop();
	else
		_sonyMaster.play();
}

void MainWindow::masterNextFrame()
{
	_sonyMaster.cue(_sonyMaster.clock()->frame() + 1, _sonyMaster.clock()->timeCodeType());
}

void MainWindow::masterPreviousFrame()
{
	_sonyMaster.cue(_sonyMaster.clock()->frame() - 1, _sonyMaster.clock()->timeCodeType());
}

void MainWindow::onDeviceIdData(unsigned char id1, unsigned char id2)
{
	QString id;
	id.sprintf("%2X %2X", id1, id2);
	ui->idLabel->setText(id);
}

void MainWindow::onStatusData(unsigned char *statusData, int offset, int length)
{
	QString statusStr = "";
	for (int i = 0; i < length; i++)
		statusStr += QString::number(statusData[i], 16) + " ";
	ui->statusLabel->setText(statusStr);
}

void MainWindow::on_masterActiveCheck_clicked(bool checked)
{
	if(checked)
	{
		PHDEBUG << "opening master";

		if(_sonyMaster.open())
		{
			PHDEBUG << "master open ok";

			_sonyMaster.deviceTypeRequest();
			_sonyMaster.statusSense();
			_sonyMaster.timeSense();
			_sonyMaster.speedSense();
		}
		else
		{
			PHDEBUG << "error opening master";
			checked = false;
			QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony master");
		}
	}
	else
	{
		PHDEBUG << "closing sony master";
		_sonyMaster.close();
	}

	ui->masterActiveCheck->setChecked(checked);

	ui->queryIdButton->setEnabled(checked);
	ui->statusSenseButton->setEnabled(checked);
	ui->timeSenseButton->setEnabled(checked);
	ui->speedSenseButton->setEnabled(checked);

	ui->masterPanel->setEnabled(checked);
}

void MainWindow::on_slaveActiveCheck_clicked(bool checked)
{
	if(checked)
	{
		if(_sonySlave.open())
			PHDEBUG << "slave open ok";
		else
		{
			PHDEBUG << "error opening slave";
			checked = false;
			QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony slave");
		}
	}
	else
	{
		PHDEBUG << "closing sony slave";
		_sonySlave.clock()->setRate(0);
		_sonySlave.close();
	}

	ui->slaveActiveCheck->setChecked(checked);
	ui->slavePanel->setEnabled(checked);
}

