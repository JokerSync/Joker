/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef TIMECODEEDIT_H
#define TIMECODEEDIT_H

#include <QWidget>
#include <QEvent>
#include <QDebug>
#include <QLineEdit>
#include <QKeyEvent>
#include "PhTools/PhTimeCode.h"

/**
 * @brief Provides an UI to edit a timecode
 */
class PhTimeCodeEdit : public QLineEdit
{
	Q_OBJECT

public:
	/**
	 * @brief PhTimeCodeEdit constructor
	 * @param parent The parent object
	 */
	explicit PhTimeCodeEdit(QWidget *parent = 0);

	/**
	 * @brief set Frame
	 *
	 * The corresponding timecode will be displayed on the window.
	 * For example, if frame = 32 and tcType = PhTimeCodeType24, the
	 * window will display 00:00:01:12
	 * @param frame the desired PhFrame
	 * @param tcType the corresponding PhTimeCodeType
	 */
	void setFrame(PhFrame frame, PhTimeCodeType tcType);

	/**
	 * @brief Check the timecode
	 * @return true if the input is correct, false otherwise
	 */
	bool isTimeCode();
	/**
	 * @brief Current frame value entered in the text field
	 * @return A frame value.
	 */
	PhFrame frame();

signals:

	/**
	 * @brief Send a signal when the text box frame changed
	 * @param frame the new frame
	 * @param tcType the new PhTimeCodeType
	 */
	void frameChanged(PhFrame frame, PhTimeCodeType tcType);

private slots:
	void onTextChanged(QString text);

private:
	PhTimeCodeType _tcType;
	bool eventFilter(QObject *sender, QEvent *event);
	QString _oldFrame;
	QString _addedNumbers;
	void compute(bool add);
};

#endif // TIMECODEEDIT_H
