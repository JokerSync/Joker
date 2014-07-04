/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhColorPickerDialog.h"
#include "ui_PhColorPickerDialog.h"

#include <QColorDialog>

PhColorPickerDialog::PhColorPickerDialog(PhStripDoc *doc, QString name, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhColorPickerDialog),
	_doc(doc),
	_name(name)
{
	ui->setupUi(this);
	// Setting the label
	ui->labelName->setText(_name);
	ui->labelName->adjustSize();

	// Saving old datas
	_currentPeople = _doc->peopleByName(_name);
	_oldColor = _currentPeople->color();

	ui->pbColor->setStyleSheet("background-color:" +  _oldColor +";"
							   "border-width: 2px;"
							   "border-radius: 10px;"
							   "border-color: beige;");

}

PhColorPickerDialog::~PhColorPickerDialog()
{
	delete ui;
}

void PhColorPickerDialog::OnColorSelected(QColor newColor) {
	// Setting the new color
	_currentPeople->setColor(newColor.name());
	_doc->setModified(true);

	ui->pbColor->setStyleSheet("background-color:" +  newColor.name() +";"
							   "border-width: 2px;"
							   "border-radius: 10px;"
							   "border-color: beige;");

}

void PhColorPickerDialog::on_buttonBox_rejected()
{
	// Reseting color
	_currentPeople->setColor(_oldColor);
}

void PhColorPickerDialog::on_pbColor_clicked()
{
		QColorDialog dlg;
		// Setting the selected color to the character's one
		dlg.setCurrentColor(QColor(_oldColor));

		// Connecting slots
		connect(&dlg, SIGNAL(currentColorChanged(QColor)), this, SLOT(OnColorSelected(QColor)));

		dlg.exec();
}
