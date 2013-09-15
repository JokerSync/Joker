#ifndef PHSONYCONTROLLERTEST_H
#define PHSONYCONTROLLERTEST_H

#include <QtTest>

#include "PhSonyController.h"

class PhSonyControllerTest : public PhSonyController
{
public:
	PhSonyControllerTest(QObject *parent = 0);

public slots:
	void testComputeRate();

protected:
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data);
};

#endif // PHSONYCONTROLLERTEST_H
