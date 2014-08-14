/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "MidiToolWindow.h"
#include "ui_MidiToolWindow.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PreferencesDialog.h"

MidiToolWindow::MidiToolWindow(MidiToolSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MidiToolWindow),
	_settings(settings),
	_mtcReader(PhTimeCodeType25),
	_mtcWriter(PhTimeCodeType25),
	_currentDigit(0),
	_lastFrame(-1),
	_frameDelta(-1),
	_lastRate(-1)
{
	ui->setupUi(this);

	on_readCheckBox_clicked(_settings->read());
	on_generateCheckBox_clicked(_settings->generate());

	_mtcWriter.clock()->setFrame(_settings->firstFrame());
	ui->widgetMaster->setMediaLength(_settings->length());
	ui->widgetMaster->setFirstFrame(_settings->firstFrame());
	ui->widgetMaster->setClock(_mtcWriter.clock());

	connect(&_clockTimer, &QTimer::timeout, this, &MidiToolWindow::onTick);

	_clockTimer.start(10);

	connect(_mtcReader.clock(), &PhClock::frameChanged, this, &MidiToolWindow::onFrameChanged);
	connect(_mtcReader.clock(), &PhClock::rateChanged, this, &MidiToolWindow::onSlaveRateChanged);
	connect(_mtcReader.clock(), &PhClock::tcTypeChanged, this, &MidiToolWindow::onSlaveTCTypeChanged);
}

MidiToolWindow::~MidiToolWindow()
{
	delete ui;
}

void MidiToolWindow::on_actionSet_TC_In_triggered()
{
//	PhTimeCodeDialog dlg(_ltcWriter.clock()->timeCodeType(), ui->widgetMaster->getFirstFrame());
//	if(dlg.exec()) {
//		ui->widgetMaster->setFirstFrame(dlg.frame());
//		_ltcWriter.clock()->setFrame(ui->widgetMaster->getFirstFrame());
//		updateInfos();
//	}
}

void MidiToolWindow::on_actionSet_TC_Out_triggered()
{
//	PhTimeCodeDialog dlg(_ltcWriter.clock()->timeCodeType(), ui->widgetMaster->getFirstFrame() + ui->widgetMaster->getMediaLength());
//	if(dlg.exec()) {
//		if(dlg.frame() > ui->widgetMaster->getFirstFrame())
//			ui->widgetMaster->setMediaLength(dlg.frame() - ui->widgetMaster->getFirstFrame());
//		else
//			PHDEBUG << "Can't set a TC Out inferior to TC In";
//		updateInfos();
//	}
}

void MidiToolWindow::on_actionPreferences_triggered()
{
//	PreferencesDialog dlg(_settings->audioOutput(), _settings->audioInput());
//	if(dlg.exec()) {
//		PHDEBUG << dlg.selectedAudioOutput();
//		_settings->setAudioOutput(dlg.selectedAudioOutput());
//		_settings->setAudioInput(dlg.selectedAudioInput());

//		if(_settings->generate())
//			setupOutput();
//		if(_settings->read())
//			setupInput();
//	}
}

void MidiToolWindow::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	ui->lblSlave->setText(PhTimeCode::stringFromFrame(frame, tcType));
}


void MidiToolWindow::onSlaveRateChanged(PhRate rate)
{
	QString s = QString("%1x since %2")
							   .arg(rate)
							   .arg(PhTimeCode::stringFromTime(_mtcReader.clock()->time(), _mtcReader.clock()->timeCodeType()));
	ui->readInfoLabel->setText(s);
}

void MidiToolWindow::onSlaveTCTypeChanged(PhTimeCodeType tcType)
{
	ui->fpsLabel->setText(QString("%0 fps").arg(PhTimeCode::getAverageFps(tcType)));
}

void MidiToolWindow::on_generateCheckBox_clicked(bool checked)
{

	ui->generatorGroupBox->setEnabled(checked);
	ui->generateCheckBox->setChecked(checked);
	_settings->setGenerate(checked);
	if(checked) {
		if(!_mtcWriter.open(_settings->outputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to send to " + _settings->inputPortName());
			on_generateCheckBox_clicked(false);
		}
	}
	else
		_mtcWriter.close();
}

void MidiToolWindow::on_readCheckBox_clicked(bool checked)
{
	ui->readerGroupBox->setEnabled(checked);
	ui->readCheckBox->setChecked(checked);
	_settings->setRead(checked);
	if(checked) {
		if(!_mtcReader.open(_settings->inputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to read from " + _settings->inputPortName());
			on_readCheckBox_clicked(false);
		}
	}
	else
		_mtcReader.close();
}

void MidiToolWindow::onTick()
{
	_mtcWriter.clock()->tick(100);
}
