/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "LTCToolWindow.h"
#include "ui_LTCToolWindow.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PreferencesDialog.h"

LTCToolWindow::LTCToolWindow(QSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	_settings(settings),
	ui(new Ui::LTCToolWindow),
	_ltcWriter(PhTimeCodeType25),
	_ltcReader(PhTimeCodeType25)
{
	ui->setupUi(this);

	on_generateCheckBox_clicked(_settings->value("generate", true).toBool());
	on_readCheckBox_clicked(_settings->value("read", true).toBool());

	_ltcWriter.clock()->setFrame(_settings->value("firstFrame", 0).toInt());
	ui->widgetMaster->setMediaLength(_settings->value("mediaLength", 1 * 60 * 25).toInt());
	ui->widgetMaster->setFirstFrame(_settings->value("firstFrame", 0).toInt());
	ui->widgetMaster->setClock(_ltcWriter.clock());

	connect(_ltcReader.clock(),  SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onSlaveFrameChanged(PhFrame,PhTimeCodeType)));

	updateInfos();

	connect(_ltcWriter.clock(), SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
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

	_settings->setValue("firstFrame", (int) ui->widgetMaster->getFirstFrame());
	_settings->setValue("mediaLength", (int) ui->widgetMaster->getMediaLength());


	tcIn = PhTimeCode::stringFromFrame(ui->widgetMaster->getFirstFrame(), _ltcWriter.clock()->timeCodeType());
	tcOut = PhTimeCode::stringFromFrame(ui->widgetMaster->getFirstFrame() + ui->widgetMaster->getMediaLength(), _ltcWriter.clock()->timeCodeType());

	ui->generateInfoLabel->setText(tcIn + " -> " + tcOut);
}

void LTCToolWindow::on_actionPreferences_triggered()
{
	PreferencesDialog dlg(_settings->value("audioOutput").toString(), _settings->value("audioInput").toString());
	if(dlg.exec()) {
		PHDEBUG << dlg.selectedAudioOutput();
		_settings->setValue("audioOutput", dlg.selectedAudioOutput());
		_settings->setValue("audioInput", dlg.selectedAudioInput());

		if(_settings->value("generate", true).toBool())
			setupOutput();
		if(_settings->value("read", true).toBool())
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
	if(frame - _lastFrame != _frameDelta) {
		_frameDelta = frame - _lastFrame;
		ui->readInfoLabel->setText(QString::number(_frameDelta) + " / " + PhTimeCode::stringFromFrame(frame, tcType));
	}
	_lastFrame = frame;
	ui->lblSlave->setText(PhTimeCode::stringFromFrame(frame, tcType));
}

void LTCToolWindow::setupOutput()
{
	_ltcWriter.close();
	if(!_ltcWriter.init(_settings->value("audioOutput", "").toString())) {
		QMessageBox::warning(this, tr("Error"),
		                     tr("Error while loading the output device.\n"
		                        "See log for more informations"),
		                     QMessageBox::Ok);
	}
}

void LTCToolWindow::setupInput()
{
	_ltcReader.close();
	if(!_ltcReader.init(_settings->value("audioInput", "").toString())) {
		QMessageBox::warning(this, tr("Error"),
		                     tr("Error while loading the input device.\n"
		                        "See log for more informations"),
		                     QMessageBox::Ok);
	}
}

void LTCToolWindow::on_generateCheckBox_clicked(bool checked)
{
	ui->generatorGroupBox->setEnabled(checked);
	ui->generateCheckBox->setChecked(checked);
	_settings->setValue("generate", checked);
	if(checked)
		setupOutput();
	else
		_ltcWriter.close();
}

void LTCToolWindow::on_readCheckBox_clicked(bool checked)
{
	ui->readerGroupBox->setEnabled(checked);
	ui->readCheckBox->setChecked(checked);
	_settings->setValue("read", checked);
	if(checked)
		setupInput();
	else
		_ltcReader.close();
}
