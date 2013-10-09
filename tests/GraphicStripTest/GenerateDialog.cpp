/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "GenerateDialog.h"
#include "ui_GenerateDialog.h"
#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"

GenerateDialog::GenerateDialog(PhStripDoc * doc, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GenerateDialog)
{
	this->doc = doc;
	ui->setupUi(this);
	ui->labelInfoNb->setText("0");
	//Text.length() * 1.20588 + 1
	ui->lineEditText->setText("Per hoc minui studium suum existimans Paulus.");
	connect(ui->lineEditText, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
	connect(ui->spinBoxNbText, SIGNAL(valueChanged(int)), this, SLOT(onTextChanged()));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

GenerateDialog::~GenerateDialog()
{
	delete ui;
}

void GenerateDialog::onTextChanged()
{
	float nbFrames = (this->ui->lineEditText->text().length() * 1.20588 + 1) * this->ui->spinBoxNbText->value();

	this->ui->labelInfoNb->setText(PhTimeCode::stringFromFrame(nbFrames, PhTimeCodeType25));
}

void GenerateDialog::onAccept()
{
	int nbLoop = this->ui->spinBoxNbLoop->value();
	int nbPeople = this->ui->spinBoxNbPeople->value();
	int nbText = this->ui->spinBoxNbText->value();
	int nbTracks = this->ui->spinBoxNbTrack->value();
	QString text = this->ui->lineEditText->text();
	doc->createDoc(text, nbPeople, nbLoop, nbText, nbTracks, 3);
}
