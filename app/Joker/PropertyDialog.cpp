/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PropertyDialog.h"
#include "ui_PropertyDialog.h"

PropertyDialog::PropertyDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PropertyDialog),
	_doc(NULL)
{
	ui->setupUi(this);
}

PropertyDialog::~PropertyDialog()
{
	delete ui;
}

void PropertyDialog::setDoc(PhStripDoc *doc)
{
	_doc = doc;
}

void PropertyDialog::setVideoEngine(PhVideoEngine *videoEngine)
{
	_videoEngine = videoEngine;
}

void PropertyDialog::showEvent(QShowEvent *)
{
	ui->titleLabel->setText("-");
	ui->tcInLabel->setText("-");
	ui->tcOutLabel->setText("-");
	ui->authorLabel->setText("-");
	ui->peopleNumberLabel->setText("-");
	ui->charNumberLabel->setText("-");

	ui->videoFileLabel->setText("-");
	ui->videoTCInLabel->setText("-");
	ui->videoTCOutLabel->setText("-");
	ui->resolutionLabel->setText("-");
	ui->fpsLabel->setText("-");
	ui->codecNameLabel->setText("-");

	if(_doc) {
		ui->titleLabel->setText(_doc->title());

		if(_doc->authorName().length())
			ui->authorLabel->setText(_doc->authorName());

		PhFrame frameIn = _doc->frameIn();
		if(frameIn > 0)
			ui->tcInLabel->setText(PhTimeCode::stringFromFrame(frameIn, _doc->timeCodeType()));

		PhFrame frameOut = _doc->frameOut();
		if(frameOut > 0)
			ui->tcOutLabel->setText(PhTimeCode::stringFromFrame(frameOut, _doc->timeCodeType()));

		int peopleNumber = _doc->peoples().count();
		ui->peopleNumberLabel->setText(QString::number(peopleNumber));

		int charNumber = 0;
		foreach(PhStripText * text, _doc->texts())
		charNumber += text->content().length();
		ui->charNumberLabel->setText(QString::number(charNumber));
	}

	if(_videoEngine) {
		if(_videoEngine->fileName().length())
			ui->videoFileLabel->setText(_videoEngine->fileName());

		if(_videoEngine->firstFrame())
			ui->videoTCInLabel->setText(PhTimeCode::stringFromFrame(_videoEngine->firstFrame(), _videoEngine->clock()->timeCodeType()));

		if(_videoEngine->length())
			ui->videoTCOutLabel->setText(PhTimeCode::stringFromFrame(_videoEngine->firstFrame() + _videoEngine->length(), _videoEngine->clock()->timeCodeType()));

		ui->resolutionLabel->setText(QString::number(_videoEngine->width()) + "x" + QString::number(_videoEngine->height()));

		ui->fpsLabel->setText(QString::number(_videoEngine->framePerSecond()));

		if(_videoEngine->codecName().length())
			ui->codecNameLabel->setText(_videoEngine->codecName());
	}
}
