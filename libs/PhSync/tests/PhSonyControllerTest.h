#ifndef PHSONYCONTROLLERTEST_H
#define PHSONYCONTROLLERTEST_H

#include <QtTest>

#include "PhSonyController.h"

/**
 * @brief Test for the PhSonyController class.
 */
class PhSonyControllerTest : public PhSonyController
{
public:
	/**
	 * @brief PhSonyControllerTest constructor
	 *
	 * @param parent The QObject owner
	 */
	PhSonyControllerTest(QObject *parent = 0);

public slots:
	/**
	 * @brief Test some rate computation.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#shuttleFwd
	 */
	void testComputeRate();

protected:
	/**
	 * @brief Empty implementation of the processCommand abstract method.
	 *
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @param dataIn Command data.
	 */
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *dataIn);
};

#endif // PHSONYCONTROLLERTEST_H
