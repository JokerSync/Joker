/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHTIMECODEDIALOG_H
#define PHTIMECODEDIALOG_H

#include <QDialog>

#include "PhTools/PhTimeCode.h"

namespace Ui {
class PhTimeCodeDialog;
}

/**
 * @brief A modal dialog box for entering timecode.
 *
 * Launch the dialog using the TimeCodeDialog::exec() method.
 * If it return QDialog::Accepted, it means that a timecode value
 * was input and validated.
 * Otherwise the user pressed cancel.
 */
class PhTimeCodeDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief PhTimeCodeDialog constructor
	 * @param tcType Timecode type to use for string to time conversion.
	 * @param time The dialog initial time.
	 * @param parent The object owner.
	 */
	explicit PhTimeCodeDialog(PhTimeCodeType tcType, PhTime time = 0, QWidget *parent = 0);

	~PhTimeCodeDialog();

	/**
	 * @brief Current time value entered in the text field
	 * @return A time value.
	 */
	PhTime time();


private slots:
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);

private:
	Ui::PhTimeCodeDialog *ui;
};

#endif // PHTIMECODEDIALOG_H
