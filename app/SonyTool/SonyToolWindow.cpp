#include <QMessageBox>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

#include "PreferencesDialog.h"
#include "SonyToolWindow.h"
#include "ui_SonyToolWindow.h"

SonyToolWindow::SonyToolWindow() :
	ui(new Ui::SonyToolWindow),
	_sonyMaster(&_settings),
	_sonySlave(&_settings)
{
	ui->setupUi(this);

	// configure panels
	ui->masterPanel->setLength(PhTimeCode::timeFromString("00:01:00:00", _sonyMaster.timeCodeType()));

	ui->slavePanel->setLength(PhTimeCode::timeFromString("00:01:00:00", _sonySlave.timeCodeType()));

	// Connect master panel to sony master
	connect(ui->masterPanel, &PhMediaPanel::playClicked, &_sonyMaster, &PhSonyMasterController::play);
	connect(ui->masterPanel, &PhMediaPanel::pauseClicked, &_sonyMaster, &PhSonyMasterController::stop);
	connect(ui->masterPanel, &PhMediaPanel::nextFrameClicked, this, &SonyToolWindow::masterNextFrame);
	connect(ui->masterPanel, &PhMediaPanel::previousFrameClicked, this, &SonyToolWindow::masterPreviousFrame);
	connect(ui->masterPanel, &PhMediaPanel::fastForwardClicked, &_sonyMaster, &PhSonyMasterController::fastForward);
	connect(ui->masterPanel, &PhMediaPanel::rewindClicked, &_sonyMaster, &PhSonyMasterController::rewind);
	connect(ui->masterPanel, &PhMediaPanel::timeCodeTypeChanged, &_settings, &SonyToolSettings::setSonyMasterCommunicationTimeCodeType);

	// Connect sony master to MainWindow
	connect(ui->queryIdButton, &QPushButton::clicked, &_sonyMaster, &PhSonyMasterController::deviceTypeRequest);
	connect(ui->statusSenseButton, &QPushButton::clicked, &_sonyMaster, &PhSonyMasterController::statusSense);
	connect(ui->timeSenseButton, &QPushButton::clicked, &_sonyMaster, &PhSonyMasterController::timeSense);
	connect(ui->speedSenseButton, &QPushButton::clicked, &_sonyMaster, &PhSonyMasterController::speedSense);

	connect(&_sonyMaster, &PhSonyMasterController::deviceIdData, this, &SonyToolWindow::onDeviceIdData);
	connect(&_sonyMaster, &PhSonyMasterController::statusData, this, &SonyToolWindow::onStatusData);

	// connect clocks to media panel
	ui->masterPanel->setClock(_sonyMaster.timeCodeType(), NULL);
	connect(_sonyMaster.clock(), &PhClock::timeChanged, ui->masterPanel, &PhMediaPanel::onTimeChanged);
	connect(_sonyMaster.clock(), &PhClock::rateChanged, ui->masterPanel, &PhMediaPanel::onRateChanged);

	ui->slavePanel->setClock(_sonySlave.timeCodeType(), NULL);
	connect(_sonySlave.clock(), &PhClock::timeChanged, ui->slavePanel, &PhMediaPanel::onTimeChanged);
	connect(_sonySlave.clock(), &PhClock::rateChanged, ui->slavePanel, &PhMediaPanel::onRateChanged);

	// start master and slave
	on_masterActiveCheck_clicked(_settings.sonyMasterActive());
	on_slaveActiveCheck_clicked(_settings.sonySlaveActive());

	ui->actionSlave_Use_video_sync->setChecked(_settings.useVideoSlaveSync());
	ui->actionMaster_Use_video_sync->setChecked(_settings.useVideoMasterSync());

	on_actionSlave_Use_video_sync_triggered(_settings.useVideoSlaveSync());
	on_actionMaster_Use_video_sync_triggered(_settings.useVideoMasterSync());

	_sonySlave.clock()->setTime(PhTimeCode::timeFromString("00:01:00:00", _sonySlave.timeCodeType()));
	connect(ui->slavePanel, &PhMediaPanel::timeCodeTypeChanged, &_settings, &SonyToolSettings::setSonySlaveCommunicationTimeCodeType);
}

SonyToolWindow::~SonyToolWindow()
{
	_sonyMaster.close();
	_sonySlave.close();
	delete ui;
}

void SonyToolWindow::masterNextFrame()
{
	_sonyMaster.cue(_sonyMaster.clock()->time() + PhTimeCode::timePerFrame(_sonyMaster.timeCodeType()));
}

void SonyToolWindow::masterPreviousFrame()
{
	_sonyMaster.cue(_sonyMaster.clock()->time() - PhTimeCode::timePerFrame(_sonyMaster.timeCodeType()));
}

void SonyToolWindow::onDeviceIdData(unsigned char id1, unsigned char id2)
{
	QString id;
	id.sprintf("%2X %2X", id1, id2);
	ui->idLabel->setText(id);
}

void SonyToolWindow::onStatusData(unsigned char *statusData, int offset, int length)
{
	Q_UNUSED(offset);
	QString statusStr = "";
	for (int i = 0; i < length; i++)
		statusStr += QString::number(statusData[i], 16) + " ";
	ui->statusLabel->setText(statusStr);
}

void SonyToolWindow::on_masterActiveCheck_clicked(bool checked)
{
	_settings.setSonyMasterActive(checked);
	if(checked) {
		PHDEBUG << "opening master";

		if(_sonyMaster.open()) {
			PHDEBUG << "master open ok";

			_sonyMaster.deviceTypeRequest();
			_sonyMaster.statusSense();
			_sonyMaster.timeSense();
			_sonyMaster.speedSense();
		}
		else {
			PHDEBUG << "error opening master";
			checked = false;
			QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony master");
		}
	}
	else {
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

void SonyToolWindow::on_slaveActiveCheck_clicked(bool checked)
{
	_settings.setSonySlaveActive(checked);
	if(checked) {
		if(_sonySlave.open())
			PHDEBUG << "slave open ok";
		else {
			PHDEBUG << "error opening slave";
			checked = false;
			QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony slave");
		}
	}
	else {
		PHDEBUG << "closing sony slave";
		_sonySlave.clock()->setRate(0);
		_sonySlave.close();
	}

	ui->slaveActiveCheck->setChecked(checked);
	ui->slavePanel->setEnabled(checked);
}


void SonyToolWindow::on_actionMaster_GoTo_triggered()
{
	PhTimeCodeDialog dlg(_sonyMaster.timeCodeType(), _sonyMaster.clock()->time());
	if(dlg.exec() == QDialog::Accepted)
		_sonyMaster.cue(dlg.time());
}

void SonyToolWindow::on_actionSlave_GoTo_triggered()
{
	PhTimeCodeDialog dlg(_sonySlave.timeCodeType(), _sonySlave.clock()->time());
	if(dlg.exec() == QDialog::Accepted)
		_sonySlave.clock()->setTime(dlg.time());
}

void SonyToolWindow::on_actionSlave_Use_video_sync_triggered(bool useVideo)
{
	_settings.setUseVideoSlaveSync(useVideo);

	_slaveTimer.stop();
	if(useVideo) {
		// timer trigger the checkVideoSync on the serial port
		disconnect(&_slaveTimer, &QTimer::timeout, &_sonySlave, &PhSonySlaveController::onVideoSync);
		connect(&_slaveTimer, &QTimer::timeout, &_sonySlave, &PhSonySlaveController::checkVideoSync);

		_slaveTimer.start(10);
	}
	else {
		// timer trigger the onVideoSync slot directly
		disconnect(&_slaveTimer, SIGNAL(timeout()), &_sonySlave, SLOT(checkVideoSync()));
		connect(&_slaveTimer, SIGNAL(timeout()), &_sonySlave, SLOT(onVideoSync()));

		_slaveTimer.start(40);
	}
}

void SonyToolWindow::on_actionMaster_Use_video_sync_triggered(bool useVideo)
{
	_settings.setUseVideoMasterSync(useVideo);

	_masterTimer.stop();
	if(useVideo) {
		// timer trigger the checkVideoSync on the serial port
		disconnect(&_masterTimer, SIGNAL(timeout()), &_sonyMaster, SLOT(onVideoSync()));
		connect(&_masterTimer, SIGNAL(timeout()), &_sonyMaster, SLOT(checkVideoSync()));

		_masterTimer.start(10);
	}
	else {
		// timer trigger the onVideoSync slot directly
		disconnect(&_masterTimer, SIGNAL(timeout()), &_sonyMaster, SLOT(checkVideoSync()));
		connect(&_masterTimer, SIGNAL(timeout()), &_sonyMaster, SLOT(onVideoSync()));

		_masterTimer.start(40);
	}
}

void SonyToolWindow::on_actionPreferences_triggered()
{
	PreferencesDialog dlg(this, &_settings);
	dlg.exec();
}
