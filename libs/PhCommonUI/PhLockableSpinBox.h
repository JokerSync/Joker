/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHLOCKABLESPINBOX_H
#define PHLOCKABLESPINBOX_H

#include "PhTools/PhGeneric.h"
#include "PhCommonUI/PhUI.h"

/**
 * @brief A lockable spin box
 *
 * This spin box is disabled by default: click eight times on it to enable.
 */
class PhLockableSpinBox : public QSpinBox
{
	Q_OBJECT
public:
	/**
	 * @brief PhLockableSpinBox constructor
	 * @param parent The parent widget
	 */
	explicit PhLockableSpinBox(QWidget *parent = 0);

protected:
	/**
	 * @brief Filter the mouse button press event
	 * @param sender The sender object
	 * @param event The event
	 *
	 * Each times the spin box is clicked, check the last click.
	 * If the last click was within the last 250 ms, increment the counter.
	 * If the counter is equal to eight, enable the spin box.
	 *
	 * @return Always false
	 */
	bool eventFilter(QObject *sender, QEvent *event);

private:
	int _pressCounter;
	QTime _pressTimer;
};

#endif // PHLOCKABLESPINBOX_H
