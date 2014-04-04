/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "LTCToolWindow.h"
#include "ui_LTCToolWindow.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PreferencesDialog.h"

LTCToolWindow::LTCToolWindow(LTCToolSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LTCToolWindow),
	_settings(settings),
	_ltcWriter(PhTimeCodeType25),
	_ltcReader(PhTimeCodeType25),
	_lastFrame(-1),
	_frameDelta(-1),
	_lastRate(-1)
{
	ui->setupUi(this);

	on_generateCheckBox_clicked(_settings->generate());
	on_readCheckBox_clicked(_settings->read());

	_ltcWriter.clock()->setFrame(_settings->firstFrame());
	ui->widgetMaster->setMediaLength(_settings->length());
	ui->widgetMaster->setFirstFrame(_settings->firstFrame());
	ui->widgetMaster->setClock(_ltcWriter.clock());

	connect(_ltcReader.clock(),  SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onSlaveFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_ltcReader.clock(),  SIGNAL(rateChanged(PhRate)), this, SLOT(onSlaveRateChanged(PhRate)));

	updateInfos();

	connect(_ltcWriter.clock(), SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));

	connect(&_ltcReader, SIGNAL(audioProcessed(int, int)), this, SLOT(onAudioProcessed(int, int)));

}

LTCToolWindow::~LTCToolWindow()
{
	delete ui;
}

void LTCToolWindow::on_actionSet_TC_In_triggered()
{
	PhTimeCodeDialog dlg(_ltcWriter.clock()->timeCodeType(), ui->widgetMaster->getFirstFrame());
	if(dlg.exec()) {
		ui->widgetMaster->setFirstFrame(dlg.frame());
		_ltcWriter.clock()->setFrame(ui->widgetMaster->getFirstFrame());
		updateInfos();
	}
}

void LTCToolWindow::on_actionSet_TC_Out_triggered()
{
	PhTimeCodeDialog dlg(_ltcWriter.clock()->timeCodeType(), ui->widgetMaster->getFirstFrame() + ui->widgetMaster->getMediaLength());
	if(dlg.exec()) {
		if(dlg.frame() > ui->widgetMaster->getFirstFrame())
			ui->widgetMaster->setMediaLength(dlg.frame() - ui->widgetMaster->getFirstFrame());
		else
			PHDEBUG << "Can't set a TC Out inferior to TC In";
		updateInfos();
	}
}

void LTCToolWindow::updateInfos()
{
	QString tcIn;
	QString tcOut;

	_settings->setFirstFrame((int) ui->widgetMaster->getFirstFrame());
	_settings->setLength((int) ui->widgetMaster->getMediaLength());


	tcIn = PhTimeCode::stringFromFrame(ui->widgetMaster->getFirstFrame(), _ltcWriter.clock()->timeCodeType());
	tcOut = PhTimeCode::stringFromFrame(ui->widgetMaster->getFirstFrame() + ui->widgetMaster->getMediaLength(), _ltcWriter.clock()->timeCodeType());

	ui->generateInfoLabel->setText(tcIn + " -> " + tcOut);
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

void LTCToolWindow::onFrameChanged(PhFrame frame, PhTimeCodeType)
{
	if(ui->cBoxLoop->isChecked() and frame > ui->widgetMaster->getMediaLength())
		_ltcWriter.clock()->setFrame(ui->widgetMaster->getFirstFrame());
}

void LTCToolWindow::onSlaveFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	updateSlaveInfo();
}

void LTCToolWindow::onSlaveRateChanged(PhRate rate)
{
	updateSlaveInfo();
}

void LTCToolWindow::updateSlaveInfo()
{
	PhFrame frame = _ltcReader.clock()->frame();
	PhTimeCodeType tcType = _ltcReader.clock()->timeCodeType();
	PhRate rate = _ltcReader.clock()->rate();
	if((frame - _lastFrame != _frameDelta) || (rate != _lastRate)) {
		_frameDelta = frame - _lastFrame;
		ui->readInfoLabel->setText(QString("%1 / %2 x%3")
								   .arg(_frameDelta)
								   .arg(PhTimeCode::stringFromFrame(frame, tcType))
								   .arg(_ltcReader.clock()->rate()));
	}
	_lastFrame = frame;
	_lastRate = rate;

	ui->lblSlave->setText(PhTimeCode::stringFromFrame(frame, tcType));
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
