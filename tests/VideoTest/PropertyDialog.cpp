/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PropertyDialog.h"
#include "ui_PropertyDialog.h"

PropertyDialog::PropertyDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PropertyDialog)
{
	ui->setupUi(this);
}

PropertyDialog::~PropertyDialog()
{
	delete ui;
}

void PropertyDialog::setVideoEngine(PhVideoEngine *videoEngine)
{
	_videoEngine = videoEngine;
}

void PropertyDialog::showEvent(QShowEvent *)
{
	ui->videoFileLabel->setText("-");
	ui->videoTCInLabel->setText("-");
	ui->videoTCOutLabel->setText("-");
	ui->resolutionLabel->setText("-");
	ui->fpsLabel->setText("-");
	ui->codecNameLabel->setText("-");

	PhTimeCodeType tcType = _videoEngine->timeCodeType();

	if(_videoEngine) {
		if(_videoEngine->fileName().length())
			ui->videoFileLabel->setText(_videoEngine->fileName());

		if(_videoEngine->timeIn())
			ui->videoTCInLabel->setText(PhTimeCode::stringFromTime(_videoEngine->timeIn(), tcType));

		if(_videoEngine->timeIn() && _videoEngine->timeOut())
			ui->videoTCOutLabel->setText(PhTimeCode::stringFromTime(_videoEngine->timeOut(), tcType));

		ui->resolutionLabel->setText(QString::number(_videoEngine->width()) + "x" + QString::number(_videoEngine->height()));

		ui->fpsLabel->setText(QString::number(_videoEngine->framePerSecond()));

		if(_videoEngine->codecName().length())
			ui->codecNameLabel->setText(_videoEngine->codecName());
	}
}
