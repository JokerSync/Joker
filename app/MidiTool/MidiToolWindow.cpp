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
	_mtcReader((PhTimeCodeType)settings->writerTimeCodeType()),
	_mtcWriter((PhTimeCodeType)settings->writerTimeCodeType()),
	_currentDigit(0),
	_lastFrame(-1),
	_frameDelta(-1),
	_lastRate(-1)
{
	ui->setupUi(this);

	on_checkBoxReadMTC_clicked(_settings->readMTC());
	on_checkBoxWriteMTC_clicked(_settings->writeMTC());

	_mtcWriter.clock()->setTime(_settings->writerTimeIn());
	ui->widgetMaster->setLength(_settings->writerLoopLength());
	ui->widgetMaster->setTimeIn(_settings->writerTimeIn());
	ui->widgetMaster->setClock(_mtcWriter.clock());

	connect(&_clockTimer, &QTimer::timeout, this, &MidiToolWindow::onTick);

	_clockTimer.start(10);

	connect(_mtcWriter.clock(), &PhClock::tcTypeChanged, this, &MidiToolWindow::updateTCTypeSetting);
	connect(_mtcWriter.clock(), &PhClock::rateChanged, this, &MidiToolWindow::updateRateSetting);

	connect(_mtcReader.clock(), &PhClock::timeChanged, this, &MidiToolWindow::onReaderTimeChanged);
	connect(_mtcReader.clock(), &PhClock::rateChanged, this, &MidiToolWindow::onReaderRateChanged);
	connect(_mtcReader.clock(), &PhClock::tcTypeChanged, this, &MidiToolWindow::updateFpsLabel);

	updateFpsLabel(_mtcReader.clock()->timeCodeType());
	updateWriterInfoLabel();

	_mtcWriter.clock()->setRate(_settings->writerRate());
}

MidiToolWindow::~MidiToolWindow()
{
	delete ui;
}

void MidiToolWindow::on_actionSet_TC_In_triggered()
{
#warning /// @todo change to time
	PhTimeCodeDialog dlg(_mtcWriter.clock()->timeCodeType(), _settings->writerTimeIn() / PhTimeCode::timePerFrame(_mtcWriter.clock()->timeCodeType()));
	if(dlg.exec()) {
		_settings->setWriterTimeIn(dlg.time());
		ui->widgetMaster->setTimeIn(dlg.time());
		_mtcWriter.clock()->setTime(dlg.time());
		updateWriterInfoLabel();
	}
}

void MidiToolWindow::on_actionSet_TC_Out_triggered()
{
	PhTimeCodeDialog dlg(_mtcWriter.clock()->timeCodeType(), (_settings->writerTimeIn() + _settings->writerLoopLength()) / PhTimeCode::timePerFrame(_mtcWriter.clock()->timeCodeType()));
	if(dlg.exec()) {
		if(dlg.time() > _settings->writerTimeIn()) {
			PhTime length = dlg.time() - _settings->writerTimeIn();
			_settings->setWriterLoopLength(length);
			ui->widgetMaster->setLength(length);
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
	on_checkBoxReadMTC_clicked(_settings->readMTC());
	on_checkBoxWriteMTC_clicked(_settings->writeMTC());
}

void MidiToolWindow::on_checkBoxWriteMTC_clicked(bool checked)
{
	ui->checkBoxWriteMTC->setChecked(checked);
	ui->groupBoxWriter->setEnabled(checked);
	_settings->setWriteMTC(checked);
	if(checked) {
		if(!_mtcWriter.open(_settings->midiOutputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to open " + _settings->midiOutputPortName());
			on_checkBoxWriteMTC_clicked(false);
		}
	}
	else
		_mtcWriter.close();
}

void MidiToolWindow::on_checkBoxReadMTC_clicked(bool checked)
{
	ui->checkBoxReadMTC->setChecked(checked);
	ui->groupBoxReader->setEnabled(checked);
	_settings->setReadMTC(checked);
	if(checked) {
		if(!_mtcReader.open(_settings->midiInputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to create " + _settings->midiInputPortName());
			on_checkBoxReadMTC_clicked(false);
		}
	}
	else
		_mtcReader.close();
}

void MidiToolWindow::updateTCTypeSetting(PhTimeCodeType tcType)
{
	_settings->setWriterTimeCodeType(tcType);
}

void MidiToolWindow::updateRateSetting(PhRate rate)
{
	_settings->setWriterRate(rate);
}

void MidiToolWindow::onReaderTimeChanged(PhTime time)
{
	ui->labelReaderTimeCode->setText(PhTimeCode::stringFromTime(time, _mtcReader.clock()->timeCodeType()));
	int delay = (_mtcWriter.clock()->time() - _mtcReader.clock()->time()) / 24;
	ui->labelDelay->setText(QString("%0 ms").arg(delay));
}

void MidiToolWindow::onReaderRateChanged(PhRate rate)
{
	QString s = QString("%1x since %2")
	            .arg(rate)
	            .arg(PhTimeCode::stringFromTime(_mtcReader.clock()->time(), _mtcReader.clock()->timeCodeType()));
	ui->readInfoLabel->setText(s);
}

void MidiToolWindow::updateFpsLabel(PhTimeCodeType tcType)
{
	ui->fpsLabel->setText(QString("%0 fps").arg(PhTimeCode::getAverageFps(tcType)));
}

void MidiToolWindow::onTick()
{
	_mtcWriter.clock()->tick(100);
}

void MidiToolWindow::updateWriterInfoLabel()
{
	PhTimeCodeType tcType = _mtcWriter.clock()->timeCodeType();
	QString info = QString("%0 => %1")
	               .arg(PhTimeCode::stringFromTime(_settings->writerTimeIn(), tcType))
	               .arg(PhTimeCode::stringFromTime(_settings->writerTimeIn() + _settings->writerLoopLength(), tcType));
	ui->writerInfoLabel->setText(info);
}
