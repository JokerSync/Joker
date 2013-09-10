#include "PhTest.h"

#include <qmath.h>

PhTest::PhTest()
{
}

bool PhTest::testIsEqual(const char *testName, int line, double expected, double value, double error)
{
	bool result = (qAbs(expected - value) < error);
	if(!result)
		PHDEBUG << testName << " expected (" << line << ") : " << expected << " / value : " << value;
	return result;
}
