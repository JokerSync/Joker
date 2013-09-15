#include <QtMath>

#include "PhSonyControllerTest.h"
#include "PhTools/PhDebug.h"

PhSonyControllerTest::PhSonyControllerTest(QObject *parent)
	: PhSonyController("A", parent)
{
}

void PhSonyControllerTest::testComputeRate()
{
	QVERIFY(qAbs(computeRate(0)) < 0.01);
	QVERIFY(qAbs(computeRate(32) - 0.1) < 0.01);
	QVERIFY(qAbs(computeRate(64) - 1.0) < 0.01);
	QVERIFY(qAbs(computeRate(79) - 2.94) < 0.01);
	QVERIFY(qAbs(computeRate(118) - 48.68) < 0.01);
}

void PhSonyControllerTest::processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data)
{
	PHDEBUG << stringFromCommand(cmd1, cmd2, data);
}
