/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "GenerateDialog.h"
#include "ui_GenerateDialog.h"
#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"

GenerateDialog::GenerateDialog(QSettings * settings, PhStripDoc * doc, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GenerateDialog),
	_settings(settings)
{
	this->_doc = doc;
	ui->setupUi(this);
	ui->lineEditText->setText(_settings->value("textContent", "Per hoc minui studium suum existimans Paulus.").toString());
	ui->lineEditTimeCode->setFrame(_settings->value("startFrame", 90000).toInt(), _doc->getTCType());
	ui->spinBoxNbPeople->setValue(_settings->value("peopleNumber", 0).toInt());
	ui->spinBoxNbText->setValue(_settings->value("textNumber", 0).toInt());
	ui->spinBoxNbTrack->setValue(_settings->value("trackNumber", 4).toInt());
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


void GenerateDialog::onTextChanged()
{
	float nbFrames = (this->ui->lineEditText->text().length() * 1.20588 + 1) * this->ui->spinBoxNbText->value();

	this->ui->labelInfoDuration->setText(PhTimeCode::stringFromFrame(nbFrames, PhTimeCodeType25));
}

void GenerateDialog::onAccept()
{
	int nbLoop = this->ui->spinBoxNbLoop->value();
	int nbPeople = this->ui->spinBoxNbPeople->value();
	int nbText = this->ui->spinBoxNbText->value();
	int nbTracks = this->ui->spinBoxNbTrack->value();
	QString text = this->ui->lineEditText->text();
	int startFrame = this->ui->lineEditTimeCode->frame();
	_doc->createDoc(text, nbPeople, nbText, nbTracks, startFrame);
	_settings->setValue("startFrame", startFrame);
	_settings->setValue("peopleNumber", nbPeople);
	_settings->setValue("textNumber", nbText);
	_settings->setValue("trackNumber", nbTracks);
	_settings->setValue("textContent", text);
}
