/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef TIMECODEEDIT_H
#define TIMECODEEDIT_H

#include <QLineEdit>
#include <QStack>

#include "PhSync/PhTimeCode.h"
#include "PhTools/PhDebug.h"

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
	 * @todo remove
	 */
	void setFrame(PhFrame frame, PhTimeCodeType tcType);

	/**
	 * @brief Set the time displayed by the widget
	 * @param time A time value.
	 * @param tcType The timecode type.
	 */
	void setTime(PhTime time, PhTimeCodeType tcType);
	/**
	 * @brief Check the timecode
	 * @return true if the input is correct, false otherwise
	 */
	bool isTimeCode();
	/**
	 * @brief Current frame value entered in the text field
	 * @return A frame value.
	 * @todo remove
	 */
	PhFrame frame();

	/**
	 * @brief Current time value entered in the text field
	 * @return A time value or zero if the text is not a valid timecode.
	 */
	PhTime time();
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
	bool eventFilter(QObject *sender, QEvent *event);
	void compute(bool add);

	PhTimeCodeType _tcType;
	QString _oldTimeCode;
	QStack<QChar> _addedNumbers;
	int _selectedIndex;

	bool _mousePressed;
	QPoint _mousePressedLocation;
};

#endif // TIMECODEEDIT_H
