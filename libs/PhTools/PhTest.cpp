#include "PhTest.h"

#include <QDebug>
#include <QtMath>

PhTest::PhTest()
{
}

bool PhTest::testIsEqual(const char *testName, int line, double expected, double value, double error)
{
	bool result = (qAbs(expected - value) < error);
	if(!result)
		qDebug() << testName << " expected (" << line << ") : " << expected << " / value : " << value;
	return result;
}
