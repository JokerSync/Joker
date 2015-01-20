/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "TimeBetweenTwoFeetDialog.h"
#include "ui_TimeBetweenTwoFeetDialog.h"

TimeBetweenTwoFeetDialog::TimeBetweenTwoFeetDialog(JokerSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TimeBetweenTwoFeetDialog)
{
	_settings = settings;
	_oldTimeBetweenTwoFeet = settings->timeBetweenTwoFeet();
	ui->setupUi(this);

	ui->horizontalSlider->setValue(_oldTimeBetweenTwoFeet / 24);
	ui->spinBox->setValue(_oldTimeBetweenTwoFeet / 24);

}

TimeBetweenTwoFeetDialog::~TimeBetweenTwoFeetDialog()
{
	delete ui;
}

void TimeBetweenTwoFeetDialog::on_horizontalSlider_sliderMoved(int position)
{
	ui->spinBox->setValue(position);
	_settings->setTimeBetweenTwoFeet(position * 24);
}

void TimeBetweenTwoFeetDialog::on_spinBox_valueChanged(int value)
{
	ui->horizontalSlider->setValue(value);
	_settings->setTimeBetweenTwoFeet(value * 24);
}

void TimeBetweenTwoFeetDialog::on_buttonBox_rejected()
{
	_settings->setTimeBetweenTwoFeet(_oldTimeBetweenTwoFeet);
}
