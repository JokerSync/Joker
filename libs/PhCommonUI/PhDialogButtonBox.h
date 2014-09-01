/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHDIALOGBUTTONBOX_H
#define PHDIALOGBUTTONBOX_H

#include <QDialogButtonBox>
#include <QShowEvent>

/**
 * @brief Custom dialog box button box class
 *
 * This class customize QDialogButtonBox by adding the correct
 * default button and fix translation issue with "Ok" and "Cancel"
 */
class PhDialogButtonBox : public QDialogButtonBox
{
	Q_OBJECT
public:
	explicit PhDialogButtonBox(QWidget *parent = 0);

protected:
	/**
	 * @brief Apply the default button behaviour.
	 */
	void showEvent(QShowEvent *);

};

#endif // PHDIALOGBUTTONBOX_H
