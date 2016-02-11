/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

#include "MidiToolWindow.h"
#include "ui_MidiToolWindow.h"
#include "PreferencesDialog.h"

MidiToolWindow::MidiToolWindow(MidiToolSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MidiToolWindow),
	_settings(settings),
	_mtcReader((PhTimeCodeType)settings->writerTimeCodeType()),
	_mtcWriter((PhTimeCodeType)settings->writerTimeCodeType()),
	_currentDigit(0),
	_lastFrame(-1),
	_frameDelta(-1),
	_lastRate(-1)
{
	ui->setupUi(this);

	on_readMtcCheckBox_clicked(_settings->readMTC());
	on_writeMtcCheckBox_clicked(_settings->writeMTC());

	_mtcWriter.clock()->setTime(_settings->writerTimeIn());
	ui->writerMediaPanel->setClock(_mtcWriter.timeCodeType(), _mtcWriter.clock());
	ui->writerMediaPanel->setTimeIn(_settings->writerTimeIn());
	ui->writerMediaPanel->setLength(_settings->writerLoopLength());
	ui->writerLoopPlaybackCheckBox->setChecked(_settings->looping());

	connect(&_clockTimer, &QTimer::timeout, this, &MidiToolWindow::onTick);

	_clockTimer.start(10);

	connect(ui->writerMediaPanel, &PhMediaPanel::playClicked, &_mtcWriter, &PhMidiTimeCodeWriter::sendMMCPlay);
	connect(ui->writerMediaPanel, &PhMediaPanel::pauseClicked, &_mtcWriter, &PhMidiTimeCodeWriter::sendMMCStop);
	connect(ui->writerMediaPanel, &PhMediaPanel::sliderMoved, this, &MidiToolWindow::onGoToTime);
	connect(ui->writerMediaPanel, &PhMediaPanel::timeCodeTypeChanged, this, &MidiToolWindow::updateTCTypeSetting);

	connect(_mtcWriter.clock(), &PhClock::timeChanged, this, &MidiToolWindow::onWriterTimeChanged);
	connect(_mtcWriter.clock(), &PhClock::rateChanged, this, &MidiToolWindow::updateRateSetting);

	connect(&_mtcReader, &PhMidiTimeCodeReader::timeCodeTypeChanged, this, &MidiToolWindow::updateFpsLabel);

	connect(_mtcReader.clock(), &PhClock::timeChanged, this, &MidiToolWindow::onReaderTimeChanged);
	connect(_mtcReader.clock(), &PhClock::rateChanged, this, &MidiToolWindow::onReaderRateChanged);

	updateFpsLabel(_mtcReader.timeCodeType());
	updateWriterInfoLabel();

	_mtcWriter.clock()->setRate(_settings->writerRate());

	_mtcReader.force24as2398(_settings->midiForce24as2398());
}

MidiToolWindow::~MidiToolWindow()
{
	delete ui;
}

void MidiToolWindow::on_actionSet_TC_In_triggered()
{
	PhTimeCodeDialog dlg(_mtcWriter.timeCodeType(), _settings->writerTimeIn());
	if(dlg.exec()) {
		_settings->setWriterTimeIn(dlg.time());
		ui->writerMediaPanel->setTimeIn(dlg.time());
		_mtcWriter.clock()->setTime(dlg.time());
		updateWriterInfoLabel();
	}
}

void MidiToolWindow::on_actionSet_TC_Out_triggered()
{
	PhTimeCodeDialog dlg(_mtcWriter.timeCodeType(), _settings->writerTimeIn() + _settings->writerLoopLength());
	if(dlg.exec()) {
		if(dlg.time() > _settings->writerTimeIn()) {
			PhTime length = dlg.time() - _settings->writerTimeIn();
			_settings->setWriterLoopLength(length);
			ui->writerMediaPanel->setLength(length);
			updateWriterInfoLabel();
		}
		else
			QMessageBox::critical(this, "Error", "Can't set a TC Out inferior to TC In");
	}
}

void MidiToolWindow::on_actionPreferences_triggered()
{
	_mtcReader.close();
	_mtcWriter.close();
	PreferencesDialog dlg(_settings);
	dlg.exec();
	on_readMtcCheckBox_clicked(_settings->readMTC());
	on_writeMtcCheckBox_clicked(_settings->writeMTC());
	_mtcReader.force24as2398(_settings->midiForce24as2398());
}

void MidiToolWindow::on_writeMtcCheckBox_clicked(bool checked)
{
	ui->writeMtcCheckBox->setChecked(checked);
	ui->writerGroupBox->setEnabled(checked);
	_settings->setWriteMTC(checked);
	if(checked) {
		if(!_mtcWriter.open(_settings->midiOutputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to open " + _settings->midiOutputPortName());
			on_writeMtcCheckBox_clicked(false);
		}
	}
	else {
		_mtcWriter.clock()->setRate(0.0);
		_mtcWriter.close();
	}
}

void MidiToolWindow::on_readMtcCheckBox_clicked(bool checked)
{
	ui->readMtcCheckBox->setChecked(checked);
	ui->readerGroupBox->setEnabled(checked);
	_settings->setReadMTC(checked);
	if(checked) {
		QString portName;

		if (_settings->midiInputUseExistingPort()) {
			portName = _settings->midiInputPortName();
		}
		else {
			portName = _settings->midiVirtualInputPortName();
		}

		if(!_mtcReader.open(portName)) {
			QMessageBox::critical(this, "Error", "Unable to create " + portName);
			on_readMtcCheckBox_clicked(false);
		}
	}
	else
		_mtcReader.close();
}

void MidiToolWindow::onGoToTime(PhTime time)
{
	unsigned int hhmmssff[4];
	PhTimeCodeType tcType = _mtcWriter.timeCodeType();
	PHDEBUG << PhTimeCode::stringFromTime(time, tcType);
	PhTimeCode::ComputeHhMmSsFfFromTime(hhmmssff, time, tcType);
	_mtcWriter.sendMMCGoto(hhmmssff[0], hhmmssff[1], hhmmssff[2], hhmmssff[3], tcType);
}

void MidiToolWindow::onWriterTimeChanged(PhTime time)
{
	PHDBG(2) << PhTimeCode::getAverageFps(_mtcWriter.timeCodeType()) << "/" << PhTimeCode::stringFromTime(time, _mtcWriter.timeCodeType());
	if(_settings->looping() && (time > _settings->writerTimeIn() + _settings->writerLoopLength()))
		_mtcWriter.clock()->setTime(_settings->writerTimeIn());
}

void MidiToolWindow::updateTCTypeSetting(PhTimeCodeType tcType)
{
	_mtcWriter.setTimeCodeType(tcType);
	_settings->setWriterTimeCodeType(tcType);
}

void MidiToolWindow::updateRateSetting(PhRate rate)
{
	_settings->setWriterRate(rate);
}

void MidiToolWindow::onReaderTimeChanged(PhTime time)
{
	PhTimeCodeType tcType = _mtcReader.timeCodeType();
	PHDBG(2) << PhTimeCode::getAverageFps(tcType) << "/" << PhTimeCode::stringFromTime(time, tcType);

	ui->readerTimeCodeLabel->setText(PhTimeCode::stringFromTime(time, tcType));
	int delay = (_mtcWriter.clock()->time() - _mtcReader.clock()->time()) / 24;
	ui->delayLabel->setText(QString("%0 ms").arg(delay));
}

void MidiToolWindow::onReaderRateChanged(PhRate rate)
{
	QString s = QString("%1x since %2")
	            .arg(rate)
	            .arg(PhTimeCode::stringFromTime(_mtcReader.clock()->time(), _mtcReader.timeCodeType()));
	ui->readerInfoLabel->setText(s);
}

void MidiToolWindow::updateFpsLabel(PhTimeCodeType tcType)
{
	PHDEBUG << tcType;
	ui->readerFpsLabel->setText(QString("%0 fps").arg(PhTimeCode::getAverageFps(tcType)));
}

void MidiToolWindow::onTick()
{
	_mtcWriter.clock()->elapse(PhTimeCode::timePerFrame(_mtcWriter.timeCodeType()) / 4);
}

void MidiToolWindow::updateWriterInfoLabel()
{
	QString info = QString("%0 => %1")
	               .arg(PhTimeCode::stringFromTime(_settings->writerTimeIn(), _mtcWriter.timeCodeType()))
	               .arg(PhTimeCode::stringFromTime(_settings->writerTimeIn() + _settings->writerLoopLength(), _mtcWriter.timeCodeType()));
	ui->writerInfoLabel->setText(info);
}

void MidiToolWindow::on_writerLoopPlaybackCheckBox_toggled(bool checked)
{
	_settings->setLooping(checked);
}
