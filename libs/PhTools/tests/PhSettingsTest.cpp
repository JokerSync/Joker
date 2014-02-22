#include "PhSettingsTest.h"

PhSettingsTest::PhSettingsTest(QObject *parent) :
	QObject(parent)
{
}

void PhSettingsTest::testIntSettings()
{
	setIntTest1(6);
	QCOMPARE(intTest1(), 6);
	QCOMPARE(intTest2(), 0);
	QCOMPARE(intTest3(), 4);
}

void PhSettingsTest::testBoolSettings()
{
	setBoolTest1(true);
	QVERIFY(boolTest1());
	QVERIFY(!boolTest2());
	QVERIFY(boolTest3());
}

void PhSettingsTest::testFloatSettings()
{
	setFloatTest1(-1.34f);
	QCOMPARE(floatTest1(), -1.34f);
	QCOMPARE(floatTest2(), 0.0f);
	QCOMPARE(floatTest3(), 3.14f);
}

void PhSettingsTest::testStringSettings()
{
	setStringTest1("test setString");
	QCOMPARE(stringTest1(), QString("test setString"));
	QCOMPARE(stringTest2(), QString(""));
	QCOMPARE(stringTest3(), QString("stringTest default value"));
}
