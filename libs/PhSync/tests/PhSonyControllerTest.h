/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHSONYCONTROLLERTEST_H
#define PHSONYCONTROLLERTEST_H

#include <QtTest>

/**
 * @brief Test for the PhSonyController class.
 */
class PhSonyControllerTest : public QObject
{
	Q_OBJECT
private slots:
	/**
	 * @brief Test some rate computation.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#shuttleFwd
	 */
	void testComputeRate();
};

#endif // PHSONYCONTROLLERTEST_H
