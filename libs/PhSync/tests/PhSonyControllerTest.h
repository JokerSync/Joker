#ifndef PHSONYCONTROLLERTEST_H
#define PHSONYCONTROLLERTEST_H

#include "PhTools/PhTest.h"
#include "PhSonyController.h"

class PhSonyControllerTest : PhSonyController, PhTest
{
public:
	PhSonyControllerTest(QObject *parent);

	bool testComputeRate();
	bool testAll();

protected:
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data);
};

#endif // PHSONYCONTROLLERTEST_H
