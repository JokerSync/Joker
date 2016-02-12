/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

#include "LTCToolWindow.h"
#include "ui_LTCToolWindow.h"
#include "PreferencesDialog.h"

LTCToolWindow::LTCToolWindow(LTCToolSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LTCToolWindow),
	_settings(settings),
	_writerTimeCodeType((PhTimeCodeType)settings->writerTimeCodeType()),
	_ltcWriter(_writerTimeCodeType),
	_ltcReader(settings),
	_lastTime(-1),
	_timeDelta(-1),
	_lastRate(-1)
{
	ui->setupUi(this);

	on_generateCheckBox_clicked(_settings->generate());
	on_readCheckBox_clicked(_settings->read());

	_ltcWriter.clock()->setTime(_settings->writerTimeIn());
	ui->widgetMaster->setLength(_settings->writerLoopLength());
	ui->widgetMaster->setTimeIn(_settings->writerTimeIn());
	ui->widgetMaster->setTimeCodeType(_writerTimeCodeType);
	ui->widgetMaster->setClock(_ltcWriter.clock());

	connect(ui->widgetMaster, &PhMediaPanel::timeCodeTypeChanged, this, &LTCToolWindow::onWriterTimeCodeTypeChanged);
	connect(_ltcWriter.clock(), &PhClock::timeChanged, this, &LTCToolWindow::onWriterTimeChanged);

	connect(_ltcReader.clock(), &PhClock::timeChanged, this, &LTCToolWindow::onReaderTimeChanged);
	connect(_ltcReader.clock(), &PhClock::rateChanged, this, &LTCToolWindow::onReaderRateChanged);
	connect(&_ltcReader, &PhLtcReader::timeCodeTypeChanged, this, &LTCToolWindow::onTCTypeChanged);

	connect(&_ltcReader, &PhLtcReader::audioProcessed, this, &LTCToolWindow::onAudioProcessed);

	updateInOutInfoLabel();
}

LTCToolWindow::~LTCToolWindow()
{
	delete ui;
}

void LTCToolWindow::on_actionSet_TC_In_triggered()
{
	PhTimeCodeDialog dlg(_writerTimeCodeType, _settings->writerTimeIn());
	if(dlg.exec()) {
		ui->widgetMaster->setTimeIn(dlg.time());
		_ltcWriter.clock()->setTime(dlg.time());
		_settings->setWriterTimeIn(dlg.time());
		updateInOutInfoLabel();
	}
}

void LTCToolWindow::on_actionSet_TC_Out_triggered()
{
	PhTimeCodeDialog dlg(_writerTimeCodeType, _settings->writerTimeIn() + _settings->writerLoopLength());
	if(dlg.exec()) {
		if(dlg.time() > _settings->writerTimeIn()) {
			PhTime length = dlg.time() - _settings->writerTimeIn();
			ui->widgetMaster->setLength(length);
			_settings->setWriterLoopLength(length);
		}
		else
			QMessageBox::critical(this, "Error", "Can't set a TC Out inferior to TC In");
		updateInOutInfoLabel();
	}
}

void LTCToolWindow::updateInOutInfoLabel()
{
	QString tcIn = PhTimeCode::stringFromTime(_settings->writerTimeIn(), _writerTimeCodeType);
	QString tcOut = PhTimeCode::stringFromTime(_settings->writerTimeIn() + _settings->writerLoopLength(), _writerTimeCodeType);

	ui->inOutInfoLabel->setText(tcIn + " -> " + tcOut);
}

void LTCToolWindow::on_actionPreferences_triggered()
{
	PreferencesDialog dlg(_settings->audioOutput(), _settings->audioInput());
	if(dlg.exec()) {
		PHDEBUG << dlg.selectedAudioOutput();
		_settings->setAudioOutput(dlg.selectedAudioOutput());
		_settings->setAudioInput(dlg.selectedAudioInput());

		if(_settings->generate())
			setupOutput();
		if(_settings->read())
			setupInput();
	}
}

void LTCToolWindow::onWriterTimeCodeTypeChanged(PhTimeCodeType tcType)
{
	_writerTimeCodeType = tcType;
	_settings->setWriterTimeCodeType(tcType);
}

void LTCToolWindow::onWriterTimeChanged(PhTime time)
{
	if(ui->cBoxLoop->isChecked() && (time > _settings->writerTimeIn() + _settings->writerLoopLength()))
		_ltcWriter.clock()->setTime(_settings->writerTimeIn());
}

void LTCToolWindow::onReaderTimeChanged(PhTime)
{
	updateReaderInfo();
}

void LTCToolWindow::onReaderRateChanged(PhRate)
{
	updateReaderInfo();
}

void LTCToolWindow::updateReaderInfo()
{
	PhTime time = _ltcReader.clock()->time();
	PhTimeCodeType tcType = _ltcReader.timeCodeType();
	PhRate rate = _ltcReader.clock()->rate();
	if((time - _lastTime != _timeDelta) || (rate != _lastRate)) {
		_timeDelta = time - _lastTime;
		ui->readInfoLabel->setText(QString("%1 / %2 x%3")
		                           .arg(_timeDelta)
		                           .arg(PhTimeCode::stringFromTime(time, tcType))
		                           .arg(_ltcReader.clock()->rate()));
	}
	_lastTime = time;
	_lastRate = rate;

	ui->readerTimeCodeLabel->setText(PhTimeCode::stringFromTime(time, tcType));
}

void LTCToolWindow::setupOutput()
{
	_ltcWriter.close();
	if(!_ltcWriter.init(_settings->audioOutput())) {
		QMessageBox::warning(this, tr("Error"),
		                     tr("Error while loading the output device.\n"
		                        "See log for more informations"),
		                     QMessageBox::Ok);
		on_generateCheckBox_clicked(false);
	}
}

void LTCToolWindow::setupInput()
{
	_ltcReader.close();
	if(!_ltcReader.init(_settings->audioInput())) {
		QMessageBox::warning(this, tr("Error"),
		                     tr("Error while loading the input device.\n"
		                        "See log for more informations"),
		                     QMessageBox::Ok);
		on_readCheckBox_clicked(false);
	}
}

void LTCToolWindow::on_generateCheckBox_clicked(bool checked)
{
	ui->generatorGroupBox->setEnabled(checked);
	ui->generateCheckBox->setChecked(checked);
	_settings->setGenerate(checked);
	if(checked)
		setupOutput();
	else
		_ltcWriter.close();
}

void LTCToolWindow::on_readCheckBox_clicked(bool checked)
{
	ui->readerGroupBox->setEnabled(checked);
	ui->readCheckBox->setChecked(checked);
	_settings->setRead(checked);
	if(checked)
		setupInput();
	else
		_ltcReader.close();
}

void LTCToolWindow::onAudioProcessed(int minLevel, int maxLevel)
{
	ui->minMaxLevelLabel->setText(QString("%1 / %2").arg(minLevel).arg(maxLevel));
}

void LTCToolWindow::onTCTypeChanged(PhTimeCodeType tcType) {
	ui->tcTypelabel->setText(QString::number(PhTimeCode::getAverageFps(tcType)) + "fps");
}
