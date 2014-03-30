/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef TIMECODEEDIT_H
#define TIMECODEEDIT_H

#include <QWidget>
#include <QEvent>
#include <QLineEdit>
#include <QKeyEvent>
#include <QRegExpValidator>
#include <QApplication>

#include "PhTools/PhTimeCode.h"
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
	 * @brief Update the timecode string with a time value
	 *
	 * @param time A time value
	 * @param tcType A timecode type
	 */
	void setTime(PhTime time, PhTimeCodeType tcType);

	/**
	 * @brief Check the timecode
	 * @return true if the input is correct, false otherwise
	 */
	bool isTimeCode();
	/**
	 * @brief Current time value entered in the text field
	 * @return A time value.
	 */
	PhTime time();

signals:

	/**
	 * @brief Send a signal when the text box time changed
	 * @param time A time value
	 * @param tcType A timecode type
	 */
	void timeChanged(PhTime time, PhTimeCodeType tcType);

private slots:
	void onTextChanged(QString text);

private:
	PhTimeCodeType _tcType;
	bool eventFilter(QObject *sender, QEvent *event);
	QString _oldString;
	QString _addedNumbers;
	void compute(bool add);
	int _selectedIndex;

	bool _mousePressed;
	QPoint _mousePressedLocation;

	void paintEvent(QPaintEvent *);
};

#endif // TIMECODEEDIT_H
