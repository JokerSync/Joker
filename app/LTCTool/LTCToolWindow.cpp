#include "LTCToolWindow.h"
#include "ui_LTCToolWindow.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PreferencesPanel.h"

LTCToolWindow::LTCToolWindow(QSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	_settings(settings),
	ui(new Ui::LTCToolWindow),
	_LTCWriter(PhTimeCodeType25),
	_LTCReader(PhTimeCodeType25)
{
	ui->setupUi(this);

	setupInput();
	setupOutput();
	_LTCWriter.clock()->setFrame(_settings->value("firstFrame", 0).toInt());
	ui->widgetMaster->setMediaLength(_settings->value("mediaLength", 1 * 60 * 25).toInt());
	ui->widgetMaster->setFirstFrame(_settings->value("firstFrame", 0).toInt());
	ui->widgetMaster->setClock(_LTCWriter.clock());


	connect(_LTCReader.clock(),  SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onSlaveFrameChanged(PhFrame,PhTimeCodeType)));

	updateInfos();

	connect(_LTCWriter.clock(), SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
}

LTCToolWindow::~LTCToolWindow()
{
	delete ui;
}

void LTCToolWindow::on_actionSet_TC_In_triggered()
{
	PhTimeCodeDialog dlg(_LTCWriter.clock()->timeCodeType(), ui->widgetMaster->getFirstFrame());
	if(dlg.exec()) {
		ui->widgetMaster->setFirstFrame(dlg.frame());
		_LTCWriter.clock()->setFrame(ui->widgetMaster->getFirstFrame());
		updateInfos();
	}
}

void LTCToolWindow::on_actionSet_TC_Out_triggered()
{
	PhTimeCodeDialog dlg(_LTCWriter.clock()->timeCodeType(), ui->widgetMaster->getFirstFrame() + ui->widgetMaster->getMediaLength());
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


	tcIn = PhTimeCode::stringFromFrame(ui->widgetMaster->getFirstFrame(), _LTCWriter.clock()->timeCodeType());
	tcOut = PhTimeCode::stringFromFrame(ui->widgetMaster->getFirstFrame() + ui->widgetMaster->getMediaLength(), _LTCWriter.clock()->timeCodeType());

	ui->lblInfo->setText(tcIn + " -> " + tcOut);
}

void LTCToolWindow::on_actionPreferences_triggered()
{
	PreferencesPanel dlg(_settings->value("audioOutput").toString(), _settings->value("audioInput").toString());
	if(dlg.exec()) {
		PHDEBUG << dlg.selectedAudioOutput();
		_settings->setValue("audioOutput", dlg.selectedAudioOutput());
		_settings->setValue("audioInput", dlg.selectedAudioInput());

		setupOutput();
		setupInput();
	}
}

void LTCToolWindow::onFrameChanged(PhFrame frame, PhTimeCodeType)
{
	if(ui->cBoxLoop->isChecked() and frame > ui->widgetMaster->getMediaLength())
		_LTCWriter.clock()->setFrame(ui->widgetMaster->getFirstFrame());
}

void LTCToolWindow::onSlaveFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	ui->lblSlave->setText(PhTimeCode::stringFromFrame(frame, tcType));
}

void LTCToolWindow::setupOutput()
{
	_LTCWriter.close();
	if(!_LTCWriter.init(_settings->value("audioOutput", "").toString()))
		QMessageBox::warning(this, tr("Error"),
		                     tr("Error while loading the output device.\n"
		                        "See log for more informations"),
		                     QMessageBox::Ok);
}

void LTCToolWindow::setupInput()
{
	_LTCReader.close();
	if(!_LTCReader.init(_settings->value("audioInput", "").toString()))
		QMessageBox::warning(this, tr("Error"),
		                     tr("Error while loading the input device.\n"
		                        "See log for more informations"),
		                     QMessageBox::Ok);
}
