/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhFeedbackDialog.h"
#include "ui_PhFeedbackDialog.h"


PhFeedbackDialog::PhFeedbackDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhFeedbackDialog)
{
	ui->setupUi(this);

	ui->problemLabel->setText(ui->problemLabel->text().arg(QString(PH_APP_NAME)));
}

PhFeedbackDialog::~PhFeedbackDialog()
{
	delete ui;
}
