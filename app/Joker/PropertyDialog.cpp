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

#ifdef USE_VIDEO
void PropertyDialog::setVideoEngine(PhVideoEngine *videoEngine)
{
	_videoEngine = videoEngine;
}
#endif

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

#ifdef USE_VIDEO
	PhTimeCodeType tcType = _videoEngine->timeCodeType();
#else
	PhTimeCodeType tcType = _doc->videoTimeCodeType();
#endif

	if(_doc) {
		ui->titleLabel->setText(_doc->title());

		if(_doc->authorName().length())
			ui->authorLabel->setText(_doc->authorName());

		PhTime timeIn = _doc->timeIn();
		if(timeIn > 0)
			ui->tcInLabel->setText(PhTimeCode::stringFromTime(timeIn, tcType));

		PhTime timeOut = _doc->timeOut();
		if(timeOut > 0)
			ui->tcOutLabel->setText(PhTimeCode::stringFromTime(timeOut, tcType));

		int peopleNumber = _doc->peopleModel()->rowCount();
		ui->peopleNumberLabel->setText(QString::number(peopleNumber));

		int charNumber = 0;
		QListIterator<PhStripLine*> i = _doc->lineModel()->iterator();
		while (i.hasNext()) {
			charNumber += i.next()->content().length();
		}
		ui->charNumberLabel->setText(QString::number(charNumber));
	}

#ifdef USE_VIDEO
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
#endif
}
