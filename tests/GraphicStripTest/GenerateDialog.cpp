/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "GenerateDialog.h"
#include "ui_GenerateDialog.h"
#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"

GenerateDialog::GenerateDialog(GraphicStripTestSettings *settings, PhStripDoc * doc, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GenerateDialog),
	_settings(settings)
{
	this->_doc = doc;
	ui->setupUi(this);
	ui->lineEditText->setText(_settings->textContent());
	ui->lineEditTimeCode->setFrame(_settings->startTime() / PhTimeCode::timePerFrame(_doc->timeCodeType()), _doc->timeCodeType());
	ui->spinBoxNbPeople->setValue(_settings->peopleNumber());
	ui->spaceBetweenTextSpinBox->setValue(_settings->spaceBetweenText());
	ui->spinBoxNbText->setValue(_settings->textNumber());
	ui->spinBoxNbTrack->setValue(_settings->trackNumber());
	connect(ui->lineEditText, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
	connect(ui->spinBoxNbText, SIGNAL(valueChanged(int)), this, SLOT(onTextChanged()));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

	// To update the time value
	onTextChanged();

}

GenerateDialog::~GenerateDialog()
{
	delete ui;
}

PhTime GenerateDialog::getTC()
{
	return PhTimeCode::frameFromString(this->ui->lineEditTimeCode->text(), PhTimeCodeType25);
}

bool GenerateDialog::getCheckBoxState()
{
	return ui->checkBox->isChecked();
}


void GenerateDialog::onTextChanged()
{
	float nbFrames = (this->ui->lineEditText->text().length() * 1.20588 + 1) * this->ui->spinBoxNbText->value();

	this->ui->labelInfoDuration->setText(PhTimeCode::stringFromFrame(nbFrames, PhTimeCodeType25));
}

void GenerateDialog::onAccept()
{
	int loopCount = this->ui->spinBoxNbLoop->value();
	int peopleCount = this->ui->spinBoxNbPeople->value();
	int textCount = this->ui->spinBoxNbText->value();
	PhTime spaceBetweenText = this->ui->spaceBetweenTextSpinBox->value();
	int trackCount = this->ui->spinBoxNbTrack->value();
	QString textContent = this->ui->lineEditText->text();
	PhTime timeIn = this->ui->lineEditTimeCode->frame() * PhTimeCode::timePerFrame(_doc->timeCodeType());
	_doc->generate(textContent, loopCount, peopleCount, spaceBetweenText, textCount, trackCount, timeIn);
	_settings->setStartTime(timeIn);
	_settings->setLoopNumber(loopCount);
	_settings->setPeopleNumber(peopleCount);
	_settings->setTextNumber(textCount);
	_settings->setSpaceBetweenText(spaceBetweenText);
	_settings->setTrackNumber(trackCount);
	_settings->setTextContent(textContent);

	QDialog::accept();
}
