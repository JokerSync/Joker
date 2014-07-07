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

	// Unset the Cancel button from default
	QPushButton * cancelBtn = ui->buttonBox->button(QDialogButtonBox::Cancel);
	if(cancelBtn)
	{
		cancelBtn->setAutoDefault(false);
		cancelBtn->setDefault(false);
	}

	// Set the Ok Button to default
	QPushButton * okBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
	if(okBtn)
	{
		okBtn->setAutoDefault(true);
		okBtn->setDefault(true);
		okBtn->setFocus();
	}
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

void RulerSpaceDialog::on_spinBox_valueChanged(int arg1)
{
	ui->horizontalSlider->setValue(arg1);
	_settings->setTimeBetweenRuler(arg1 * 24);
}

void RulerSpaceDialog::on_buttonBox_rejected()
{
	_settings->setTimeBetweenRuler(_oldSpace);
}
