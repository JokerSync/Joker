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
	_generatingClock(PhTimeCodeType25),
	_currentDigit(0),
	_lastFrame(-1),
	_frameDelta(-1),
	_lastRate(-1)
{
	ui->setupUi(this);

	on_readCheckBox_clicked(_settings->read());
	on_generateCheckBox_clicked(_settings->generate());

	_generatingClock.setFrame(_settings->firstFrame());
	ui->widgetMaster->setMediaLength(_settings->length());
	ui->widgetMaster->setFirstFrame(_settings->firstFrame());
	ui->widgetMaster->setClock(&_generatingClock);

	connect(&_clockTimer, &QTimer::timeout, this, &MidiToolWindow::onTick);

	_clockTimer.start(10);

	connect(&_midiInput, &PhMidiInput::timeCodeReceived, this, &MidiToolWindow::onTC);
//	connect(_ltcReader.clock(),  SIGNAL(rateChanged(PhRate)), this, SLOT(onSlaveRateChanged(PhRate)));

	updateInfos();

//	connect(_ltcWriter.clock(), SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));

//	connect(&_ltcReader, SIGNAL(audioProcessed(int, int)), this, SLOT(onAudioProcessed(int, int)));

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

void MidiToolWindow::updateInfos()
{
	QString tcIn;
	QString tcOut;

	_settings->setFirstFrame((int) ui->widgetMaster->getFirstFrame());
	_settings->setLength((int) ui->widgetMaster->getMediaLength());


//	tcIn = PhTimeCode::stringFromFrame(ui->widgetMaster->getFirstFrame(), _ltcWriter.clock()->timeCodeType());
//	tcOut = PhTimeCode::stringFromFrame(ui->widgetMaster->getFirstFrame() + ui->widgetMaster->getMediaLength(), _ltcWriter.clock()->timeCodeType());

//	ui->generateInfoLabel->setText(tcIn + " -> " + tcOut);
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

void MidiToolWindow::onFrameChanged(PhFrame frame, PhTimeCodeType)
{
//	if(ui->cBoxLoop->isChecked() and frame > ui->widgetMaster->getMediaLength())
	//		_ltcWriter.clock()->setFrame(ui->widgetMaster->getFirstFrame());
}

void MidiToolWindow::onTC(int hh, int mm, int ss, int ff, PhTimeCodeType tcType)
{
	PhFrame frame = PhTimeCode::frameFromHhMmSsFf(hh, mm, ss, ff, tcType);
	ui->lblSlave->setText(PhTimeCode::stringFromFrame(frame, tcType));
}

void MidiToolWindow::onSlaveRateChanged(PhRate rate)
{
	updateSlaveInfo();
}

void MidiToolWindow::updateSlaveInfo()
{
//	PhFrame frame = _ltcReader.clock()->frame();
//	PhTimeCodeType tcType = _ltcReader.clock()->timeCodeType();
//	PhRate rate = _ltcReader.clock()->rate();
//	if((frame - _lastFrame != _frameDelta) || (rate != _lastRate)) {
//		_frameDelta = frame - _lastFrame;
//		ui->readInfoLabel->setText(QString("%1 / %2 x%3")
//		                           .arg(_frameDelta)
//		                           .arg(PhTimeCode::stringFromFrame(frame, tcType))
//		                           .arg(_ltcReader.clock()->rate()));
//	}
//	_lastFrame = frame;
//	_lastRate = rate;

//	ui->lblSlave->setText(PhTimeCode::stringFromFrame(frame, tcType));
}

void MidiToolWindow::setupOutput()
{
//	_ltcWriter.close();
//	if(!_ltcWriter.init(_settings->audioOutput())) {
//		QMessageBox::warning(this, tr("Error"),
//		                     tr("Error while loading the output device.\n"
//		                        "See log for more informations"),
//		                     QMessageBox::Ok);
//		on_generateCheckBox_clicked(false);
//	}
}

void MidiToolWindow::on_generateCheckBox_clicked(bool checked)
{

	ui->generatorGroupBox->setEnabled(checked);
	ui->generateCheckBox->setChecked(checked);
	_settings->setGenerate(checked);
	if(checked) {
		if(!_midiOutput.open(_settings->outputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to send to " + _settings->inputPortName());
			on_generateCheckBox_clicked(false);
		}
	}
	else
		_midiOutput.close();
}

void MidiToolWindow::on_readCheckBox_clicked(bool checked)
{
	ui->readerGroupBox->setEnabled(checked);
	ui->readCheckBox->setChecked(checked);
	_settings->setRead(checked);
	if(checked) {
		if(!_midiInput.open(_settings->inputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to read from " + _settings->inputPortName());
			on_readCheckBox_clicked(false);
		}
	}
	else
		_midiInput.close();
}

void MidiToolWindow::onTick()
{
	_generatingClock.tick(100);
	unsigned int hhmmssff[4];
	PhTimeCode::ComputeHhMmSsFf(hhmmssff, _generatingClock.frame(), _generatingClock.timeCodeType());
	unsigned char mtcData = _currentDigit << 4;
	switch(_currentDigit++) {
	case 0:
		mtcData |= hhmmssff[3] & 0x0f;
		break;
	case 1:
		mtcData |= hhmmssff[3] & 0xf0;
		break;
	case 2:
		mtcData |= hhmmssff[2] & 0x0f;
		break;
	case 3:
		mtcData |= hhmmssff[2] & 0xf0;
		break;
	case 4:
		mtcData |= hhmmssff[1] & 0x0f;
		break;
	case 5:
		mtcData |= hhmmssff[1] & 0xf0;
		break;
	case 6:
		mtcData |= hhmmssff[0] & 0x0f;
		break;
	case 7:
		switch(_generatingClock.timeCodeType()) {
		case PhTimeCodeType2398:
		case PhTimeCodeType24:
			mtcData |= 0;
			break;
		case PhTimeCodeType25:
			mtcData |= 1 << 1;
			break;
		case PhTimeCodeType2997:
			mtcData |= 2 << 1;
			break;
		case PhTimeCodeType30:
			mtcData |= 3 << 1;
			break;
		}

		mtcData |= hhmmssff[0] & 0xf0;
		_currentDigit = 0;
		break;
	}

	_midiOutput.sendQFTC(mtcData);
}
