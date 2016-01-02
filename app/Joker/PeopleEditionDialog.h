/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHCOLORPICKERDIALOG_H
#define PHCOLORPICKERDIALOG_H

#include <QDialog>

#include "PhStrip/PhStripDoc.h"
#include "PhStrip/PhPeople.h"

namespace Ui {
class PhColorPickerDialog;
}

/**
 * @brief The PhColorPickerDialog class
 *
 * It provide a dialog which allow the user
 * to change a character's color
 */
class PeopleEditionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PeopleEditionDialog(PhStripDoc * doc, PhPeople * people, QWidget *parent = 0);
	~PeopleEditionDialog();

public slots:
	/**
	 * @brief On color selected
	 * @param newColor
	 *
	 * React when the user select another color
	 */
	void OnColorSelected(QColor newColor);
private slots:
	void on_buttonBox_rejected();

	void on_pbColor_clicked();

private:
	Ui::PhColorPickerDialog *ui;
	PhStripDoc * _doc;
	QString _oldColor;
	bool _oldModified;
	PhPeople * _people;
};

#endif // PHCOLORPICKERDIALOG_H
