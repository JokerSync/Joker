/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QMessageBox>
#include "PrefPanel.h"
#include "PhDebug.h"
#include "ui_PrefPanel.h"
PrefPanel::PrefPanel(QSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PrefPanel),
	_settings(settings)
{
	// Load the ui
	ui->setupUi(this);
	this->setMaximumHeight(this->height());
	this->setMaximumWidth(this->width());
	ui->tabWidget->setCurrentIndex(0);
	this->setFocus();

	// Load the old settings
	_oldUseQuarterFrame = _settings->value("useQuarterFrame", false).toBool();
	_oldDelay = _settings->value("delay", 0).toInt();
	_oldStripHeight = _settings->value("stripHeight", 0.25f).toFloat();
	_oldSonyAutoConnect = _settings->value("sonyAutoConnect", true).toBool();
	_oldOpenLastFile = _settings->value("openLastFile", true).toBool();
	_oldStartFullScreen = _settings->value("startFullScreen", false).toBool();
	_oldSpeed = _settings->value("speed", 12).toInt();
	_oldBolness = _settings->value("boldness", 0).toInt();

	ui->sliderBoldness->setValue(_oldBolness);
	ui->spinBoxSpeed->setValue(_oldSpeed);
	if(_oldUseQuarterFrame)
	{
		ui->cBoxQuarterFram->setChecked(true);
		ui->spinBoxDelay->setValue(_oldDelay / 10);
	}
	else
		ui->spinBoxDelay->setValue(_oldDelay);

	ui->sliderStripHeight->setValue(ui->sliderStripHeight->maximum() * _oldStripHeight);
	ui->cBoxLastFile->setChecked(_oldOpenLastFile);
	ui->cBoxFullscreen->setChecked(_oldStartFullScreen);

	PHDEBUG;
}

PrefPanel::~PrefPanel()
{
	delete ui;
}


void PrefPanel::on_buttonBox_accepted()
{
	close();
}

void PrefPanel::on_buttonBox_rejected()
{
	_settings->setValue("useQuarterFrame", _oldUseQuarterFrame);
	_settings->setValue("delay", _oldDelay);
	_settings->setValue("stripHeight", _oldStripHeight);
	_settings->setValue("sonyAutoConnect", _oldSonyAutoConnect);
	_settings->setValue("openLastFile", _oldOpenLastFile);
	_settings->setValue("startFullScreen", _oldStartFullScreen);
	_settings->setValue("speed", _oldSpeed);
	_settings->setValue("boldness", _oldBolness);
	close();
}


void PrefPanel::on_spinBoxDelay_valueChanged(int delay)
{
	if(_settings->value("useQuarterFrame", false).toBool())
		_settings->setValue("delay", delay * 20);
	else
		_settings->setValue("delay", delay);
}

void PrefPanel::on_spinBoxSpeed_valueChanged(int speed)
{
	_settings->setValue("speed", speed);
}

void PrefPanel::on_cBoxQuarterFram_toggled(bool checked)
{
	_settings->setValue("useQuarterFrame", checked);
	if(checked)
		ui->spinBoxDelay->setValue(_settings->value("delay", 0).toInt() / 10);
	else
		ui->spinBoxDelay->setValue(_settings->value("delay", 0).toInt());
	ui->spinBoxDelay->selectAll();
}

void PrefPanel::on_sliderStripHeight_sliderMoved(int position)
{
	_settings->setValue("stripHeight", ((float)position / ui->sliderStripHeight->maximum()));
}

void PrefPanel::on_cBoxSonyAutoconnect_toggled(bool checked)
{
	_settings->setValue("sonyAutoConnect", checked);
}

void PrefPanel::on_cBoxLastFile_toggled(bool checked)
{
	_settings->setValue("openLastFile", checked);
}

void PrefPanel::on_cBoxFullscreen_toggled(bool checked)
{
	_settings->setValue("startFullScreen", checked);
}

void PrefPanel::on_sliderBoldness_sliderMoved(int position)
{
	_settings->setValue("boldness", position);
}
