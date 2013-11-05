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
	if(_oldUseQuarterFrame)
	{
		ui->cBoxQuarterFram->setChecked(true);
		ui->spinBoxDelay->setValue(_oldDelay / 10);
	}
	else
		ui->spinBoxDelay->setValue(_oldDelay);

	ui->sliderStripHeight->setValue(ui->sliderStripHeight->maximum() * _oldStripHeight);

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
	close();
}


void PrefPanel::on_spinBoxDelay_valueChanged(int delay)
{
	if(_settings->value("useQuarterFrame", false).toBool())
		_settings->setValue("delay", delay * 20);
	else
		_settings->setValue("delay", delay);
}

void PrefPanel::on_cBoxQuarterFram_toggled(bool checked)
{
	_settings->setValue("useQuarterFrame", checked);
	if(checked)
		ui->spinBoxDelay->setValue(_settings->value("delay", 0).toInt() / 20);
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
