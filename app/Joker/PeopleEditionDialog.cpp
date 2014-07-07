/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PeopleEditionDialog.h"
#include "ui_PeopleEditionDialog.h"

#include <QColorDialog>

PeopleEditionDialog::PeopleEditionDialog(PhStripDoc *doc, PhPeople * people, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhColorPickerDialog),
	_doc(doc),
	_people(people)
{
	ui->setupUi(this);
	// Setting the label
	ui->labelName->setText(_people->name());
	ui->labelName->adjustSize();

	// Saving old datas
	_oldColor = _people->color();

	ui->pbColor->setStyleSheet("background-color:" +  _oldColor +";"
	                           "border-width: 2px;"
	                           "border-radius: 10px;"
	                           "border-color: beige;");

}

PeopleEditionDialog::~PeopleEditionDialog()
{
	delete ui;
}

void PeopleEditionDialog::OnColorSelected(QColor newColor) {
	// Setting the new color
	_people->setColor(newColor.name());
	_doc->setModified(true);

	ui->pbColor->setStyleSheet("background-color:" +  newColor.name() +";"
	                           "border-width: 2px;"
	                           "border-radius: 10px;"
	                           "border-color: beige;");

}

void PeopleEditionDialog::on_buttonBox_rejected()
{
	// Reseting color
	_people->setColor(_oldColor);
}

void PeopleEditionDialog::on_pbColor_clicked()
{
	QColorDialog dlg;
	// Setting the selected color to the character's one
	dlg.setCurrentColor(QColor(_oldColor));

	// Connecting slots
	connect(&dlg, SIGNAL(currentColorChanged(QColor)), this, SLOT(OnColorSelected(QColor)));

	dlg.exec();
}
