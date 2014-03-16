#include "SettingsTest.h"

SettingsTest::SettingsTest(QObject *parent) :
	QObject(parent)
{
}

void SettingsTest::initTestCase()
{
	this->clear();
}

void SettingsTest::testClear()
{
	setIntTest1(1);
	setUnsignedCharTest1('a');
	setBoolTest1(true);
	setFloatTest1(0.12f);
	setStringTest1("coucou");

	QStringList list;
	list.append("alpha");
	list.append("beta");
	list.append("gamma");
	setStringListTest1(list);

	QByteArray array;
	array.append('a');
	array.append('b');
	array.append('c');
	array.append('b');
	setByteArrayTest1(array);

	// Clear the settings
	this->clear();

	QCOMPARE(intTest1(), 0);
	QCOMPARE((int)unsignedCharTest1(), 0);
	QVERIFY(!boolTest1());
	QCOMPARE(floatTest1(), 0.0f);
	QCOMPARE(stringTest1(), QString(""));
	QCOMPARE(stringListTest1().count(), 0);
	QCOMPARE(byteArrayTest1().count(), 0);
}

void SettingsTest::testIntSettings()
{
	setIntTest1(6);
	QCOMPARE(intTest1(), 6);
	QCOMPARE(intTest2(), 0);
	QCOMPARE(intTest3(), 4);
	QCOMPARE(intTest4(), 6);
	setIntTest4(8);
	QCOMPARE(intTest1(), 6);
	QCOMPARE(intTest4(), 8);
}

void SettingsTest::testUnsignedCharSettings()
{
	setUnsignedCharTest1(0x11);
	QCOMPARE((int)unsignedCharTest1(), 0x11);
	QCOMPARE((int)unsignedCharTest2(), 0x0);
	QCOMPARE((int)unsignedCharTest3(), 0xF0);
}

void SettingsTest::testBoolSettings()
{
	setBoolTest1(true);
	QVERIFY(boolTest1());
	QVERIFY(!boolTest2());
	QVERIFY(boolTest3());
}

void SettingsTest::testFloatSettings()
{
	setFloatTest1(-1.34f);
	QCOMPARE(floatTest1(), -1.34f);
	QCOMPARE(floatTest2(), 0.0f);
	QCOMPARE(floatTest3(), 3.14f);
}

void SettingsTest::testStringSettings()
{
	setStringTest1("test setString");
	QCOMPARE(stringTest1(), QString("test setString"));
	QCOMPARE(stringTest2(), QString(""));
	QCOMPARE(stringTest3(), QString("stringTest default value"));
}

void SettingsTest::testStringListSettings()
{
	// Test empty string list
	QCOMPARE(0, stringListTest1().size());

	// Test changing string list settings

	setStringTest4("Check other settings are not affected by string list settings");
	QStringList list1;
	list1.append("a");
	list1.append("b");
	list1.append("c");
	list1.append("d");

	setStringListTest2(list1);

	QCOMPARE(stringTest4(), QString("Check other settings are not affected by string list settings"));

	QStringList list2 = stringListTest2();

	QCOMPARE(list2.size(), list1.size());

	for(int i=0; i< list1.size();i++)
		QCOMPARE(list2.at(i), list1.at(i));

	// Test changin an existing list settings
	QStringList list3;
	list3.append("e");
	list3.append("f");
	list3.append("g");

	setStringListTest2(list3);

	QCOMPARE(stringTest4(), QString("Check other settings are not affected by string list settings"));

	QStringList list4 = stringListTest2();

	QCOMPARE(list3.size(), list4.size());

	for(int i=0; i< list3.size();i++)
		QCOMPARE(list4.at(i), list3.at(i));
}

void SettingsTest::testByteArraySettings()
{
	// Test empty array
	QCOMPARE(0, byteArrayTest1().size());

	// Test changing array settings
	QByteArray array1;
	array1.append('t');
	array1.append('e');
	array1.append('s');
	array1.append('t');

	setByteArrayTest2(array1);

	QByteArray array2 = byteArrayTest2();

	QCOMPARE(array2.size(), array1.size());

	for(int i=0; i< array1.size();i++)
		QCOMPARE(array2.at(i), array1.at(i));
}
