#ifndef PHTEST_H
#define PHTEST_H

#define PH_TEST_START bool phTestResult = true
#define PH_TEST_IS_EQUAL(expected, value, error) phTestResult += testIsEqual(__FUNCTION__, __LINE__, expected, value, error)
#define PH_TEST_END qDebug() << __FUNCTION__ << " : " << (phTestResult?"Success":"Failed"); return phTestResult

class PhTest
{
public:
	PhTest();

	bool testIsEqual(const char * testName, int line, double expected, double value, double error);
};

#endif // PHTEST_H

