/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
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
	 * @param tcType Timecode type to use for string to frame conversion.
	 * @param frame The dialog initial frame.
	 * @param parent The object owner.
	 */
	explicit PhTimeCodeDialog(PhTimeCodeType tcType, PhFrame frame = 0, QWidget *parent = 0);

	~PhTimeCodeDialog();

	/**
	 * @brief Current frame value entered in the text field
	 * @return A frame value.
	 */
	PhFrame frame();


private slots:
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);

private:
	Ui::PhTimeCodeDialog *ui;
};

#endif // PHTIMECODEDIALOG_H
