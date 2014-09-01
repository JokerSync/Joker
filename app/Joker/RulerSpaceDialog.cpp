/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "RulerSpaceDialog.h"
#include "ui_RulerSpaceDialog.h"

#include <QPushButton>

RulerSpaceDialog::RulerSpaceDialog(JokerSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RulerSpaceDialog)
{
	_settings = settings;
	_oldSpace = settings->timeBetweenRuler();
	ui->setupUi(this);

	ui->horizontalSlider->setValue(_oldSpace / 24);
	ui->spinBox->setValue(_oldSpace / 24);

}

RulerSpaceDialog::~RulerSpaceDialog()
{
	delete ui;
}

void RulerSpaceDialog::on_horizontalSlider_sliderMoved(int position)
{
	ui->spinBox->setValue(position);
	_settings->setTimeBetweenRuler(position * 24);
}

void RulerSpaceDialog::on_spinBox_valueChanged(int value)
{
	ui->horizontalSlider->setValue(value);
	_settings->setTimeBetweenRuler(value * 24);
}

void RulerSpaceDialog::on_buttonBox_rejected()
{
	_settings->setTimeBetweenRuler(_oldSpace);
}
