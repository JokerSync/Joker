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
	void processCommand(char cmd1, char cmd2, const char *data);
};

#endif // PHSONYCONTROLLERTEST_H
