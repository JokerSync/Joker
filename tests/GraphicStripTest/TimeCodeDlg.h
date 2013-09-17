#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>

#include "PhTools/PhTimeCode.h"
#ifndef DIALOG_H
#define DIALOG_H

/**
 * @brief A modal dialog box for entering timecode.
 *
 * Launch the dialog using the TimeCodeDlg::exec() method.
 * If it return QDialog::Accepted, it means that a timecode value
 * was input and validated.
 * Otherwise the user pressed cancel.
 */
class TimeCodeDlg : public QDialog
{
public:
	/**
	 * @brief TimeCodeDlg constructor
	 * @param tcType Timecode type to use for string to frame conversion.
	 * @param frame The dialog initial frame.
	 * @param parent The object owner.
	 */
	TimeCodeDlg(PhTimeCodeType tcType, PhFrame frame = 0, QWidget *parent=0);

	/**
	 * @brief Current frame value entered in the text field
	 * @return A frame value.
	 */
	PhFrame frame();
private:
	PhTimeCodeType _tcType;
	QLineEdit * textLineEdit;
};

#endif // DIALOG_H
