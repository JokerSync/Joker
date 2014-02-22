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

void PhSettingsTest::testStringListSettings()
{
	setStringTest4("Check other settings are not affected by string list settings");
	QStringList list1;
	list1.append("a");
	list1.append("b");
	list1.append("c");
	list1.append("d");

	setStringListTest(list1);

	QCOMPARE(stringTest4(), QString("Check other settings are not affected by string list settings"));

	QStringList list2 = stringListTest();

	QCOMPARE(list2.size(), list1.size());

	for(int i=0; i< list1.size();i++)
		QCOMPARE(list2.at(i), list1.at(i));

	QStringList list3;
	list3.append("e");
	list3.append("f");
	list3.append("g");

	setStringListTest(list3);

	QCOMPARE(stringTest4(), QString("Check other settings are not affected by string list settings"));

	QStringList list4 = stringListTest();

	QCOMPARE(list3.size(), list4.size());

	for(int i=0; i< list3.size();i++)
		QCOMPARE(list4.at(i), list3.at(i));
}
