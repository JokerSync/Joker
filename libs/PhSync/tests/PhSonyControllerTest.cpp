#include "PhSonyControllerTest.h"

PhSonyControllerTest::PhSonyControllerTest(QObject *parent)
	: PhSonyController("A", parent)
{
}

bool PhSonyControllerTest::testComputeRate()
{
	PH_TEST_START;

	PH_TEST_IS_EQUAL(0.0, computeRate(0), 0.01);
	PH_TEST_IS_EQUAL(0.1, computeRate(32), 0.01);
	PH_TEST_IS_EQUAL(1.0, computeRate(64), 0.01);
	PH_TEST_IS_EQUAL(2.94, computeRate(79), 0.01);
	PH_TEST_IS_EQUAL(48.69, computeRate(118), 0.01);

	PH_TEST_END;
}

bool PhSonyControllerTest::testAll()
{
	bool result = true;
	result &= testComputeRate();

	qDebug() << __FUNCTION__ << " result : " << result;

	return result;
}

void PhSonyControllerTest::processCommand(char cmd1, char cmd2, const char *data)
{
	qDebug() << stringFromCommand(cmd1, cmd2, data);
}
