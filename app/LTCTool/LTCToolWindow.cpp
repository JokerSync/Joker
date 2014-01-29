#include "LTCToolWindow.h"
#include "ui_LTCToolWindow.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PreferencesPanel.h"

LTCToolWindow::LTCToolWindow(QSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	_settings(settings),
	ui(new Ui::LTCToolWindow),
	_LTCWriter(PhTimeCodeType25)
{
	ui->setupUi(this);

	setupOutput();
	_clock = _LTCWriter.clock();
	ui->widget->setMediaLength(1 * 60 * 25);
	ui->widget->setTCType(_clock->timeCodeType());
	ui->widget->setFirstFrame(_clock->frame());
	ui->widget->setClock(_clock);
	updateInfos();

	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));

	connect(_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), &_LTCWriter, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	_timer->start(10);
}

LTCToolWindow::~LTCToolWindow()
{
	delete ui;
}

void LTCToolWindow::updateFrame()
{
	if(ui->cBoxLoop->isChecked() and _clock->frame() > ui->widget->getMediaLength())
		_clock->setFrame(ui->widget->getFirstFrame());

	_clock->tick(100);
}

void LTCToolWindow::on_actionSet_TC_In_triggered()
{
	PhTimeCodeDialog dlg(_clock->timeCodeType());
	if(dlg.exec())
	{
		ui->widget->setFirstFrame(dlg.frame());
		_clock->setFrame(ui->widget->getFirstFrame());
		updateInfos();
	}
}

void LTCToolWindow::on_actionSet_TC_Out_triggered()
{
	PhTimeCodeDialog dlg(_clock->timeCodeType());
	if(dlg.exec())
	{
		if(dlg.frame() > ui->widget->getFirstFrame())
			ui->widget->setMediaLength(dlg.frame() - ui->widget->getFirstFrame());
		else
			PHDEBUG << "Can't set a TC Out inferior to TC In";
		updateInfos();
	}
}

void LTCToolWindow::updateInfos()
{
	QString tcIn;
	QString tcOut;

	tcIn = PhTimeCode::stringFromFrame(ui->widget->getFirstFrame(), _clock->timeCodeType());
	tcOut = PhTimeCode::stringFromFrame(ui->widget->getFirstFrame() + ui->widget->getMediaLength(), _clock->timeCodeType());

	ui->lblInfo->setText(tcIn + " -> " + tcOut);
}

void LTCToolWindow::on_actionPreferences_triggered()
{
	PreferencesPanel dlg(_settings->value("audioOutput").toString());
	if(dlg.exec())
	{
		PHDEBUG << dlg.selectedAudioOutput();
		_settings->setValue("audioOutput", dlg.selectedAudioOutput());
		setupOutput();
	}
}

void LTCToolWindow::setupOutput()
{
	_LTCWriter.close();
	_LTCWriter.init(_settings->value("audioOutput", "").toString());
}
