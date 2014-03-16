#include "SettingsTest.h"
#include "AutoTestSettings.h"

void SettingsTest::testClear()
{
	AutoTestSettings settings;

	settings.setIntTest1(1);
	settings.setUnsignedCharTest1('a');
	settings.setBoolTest1(true);
	settings.setFloatTest1(0.12f);
	settings.setStringTest1("coucou");

	QStringList list;
	list.append("alpha");
	list.append("beta");
	list.append("gamma");
	settings.setStringListTest1(list);

	QByteArray array;
	array.append('a');
	array.append('b');
	array.append('c');
	array.append('b');
	settings.setByteArrayTest1(array);

	// Clear the settings
	settings.clear();

	QCOMPARE(settings.intTest1(), 0);
	QCOMPARE((int)settings.unsignedCharTest1(), 0);
	QVERIFY(!settings.boolTest1());
	QCOMPARE(settings.floatTest1(), 0.0f);
	QCOMPARE(settings.stringTest1(), QString(""));
	QCOMPARE(settings.stringListTest1().count(), 0);
	QCOMPARE(settings.byteArrayTest1().count(), 0);
}

void SettingsTest::testIntSettings()
{
	AutoTestSettings settings(true);

	settings.setIntTest1(6);
	QCOMPARE(settings.intTest1(), 6);
	QCOMPARE(settings.intTest2(), 0);
	QCOMPARE(settings.intTest3(), 4);
	QCOMPARE(settings.intTest4(), 6);
	settings.setIntTest4(8);
	QCOMPARE(settings.intTest1(), 6);
	QCOMPARE(settings.intTest4(), 8);
}

void SettingsTest::testUnsignedCharSettings()
{
	AutoTestSettings settings(true);

	settings.setUnsignedCharTest1(0x11);
	QCOMPARE((int)settings.unsignedCharTest1(), 0x11);
	QCOMPARE((int)settings.unsignedCharTest2(), 0x0);
	QCOMPARE((int)settings.unsignedCharTest3(), 0xF0);
}

void SettingsTest::testBoolSettings()
{
	AutoTestSettings settings(true);

	settings.setBoolTest1(true);
	QVERIFY(settings.boolTest1());
	QVERIFY(!settings.boolTest2());
	QVERIFY(settings.boolTest3());
}

void SettingsTest::testFloatSettings()
{
	AutoTestSettings settings(true);

	settings.setFloatTest1(-1.34f);
	QCOMPARE(settings.floatTest1(), -1.34f);
	QCOMPARE(settings.floatTest2(), 0.0f);
	QCOMPARE(settings.floatTest3(), 3.14f);
}

void SettingsTest::testStringSettings()
{
	AutoTestSettings settings(true);

	settings.setStringTest1("test setString");
	QCOMPARE(settings.stringTest1(), QString("test setString"));
	QCOMPARE(settings.stringTest2(), QString(""));
	QCOMPARE(settings.stringTest3(), QString("stringTest default value"));
}

void SettingsTest::testStringListSettings()
{
	AutoTestSettings settings(true);

	// Test empty string list
	QCOMPARE(0, settings.stringListTest1().size());

	// Test changing string list settings

	settings.setStringTest4("Check other settings are not affected by string list settings");
	QStringList list1;
	list1.append("a");
	list1.append("b");
	list1.append("c");
	list1.append("d");

	settings.setStringListTest2(list1);

	QCOMPARE(settings.stringTest4(), QString("Check other settings are not affected by string list settings"));

	QStringList list2 = settings.stringListTest2();

	QCOMPARE(list2.size(), list1.size());

	for(int i=0; i< list1.size();i++)
		QCOMPARE(list2.at(i), list1.at(i));

	// Test changin an existing list settings
	QStringList list3;
	list3.append("e");
	list3.append("f");
	list3.append("g");

	settings.setStringListTest2(list3);

	QCOMPARE(settings.stringTest4(), QString("Check other settings are not affected by string list settings"));

	QStringList list4 = settings.stringListTest2();

	QCOMPARE(list3.size(), list4.size());

	for(int i=0; i< list3.size();i++)
		QCOMPARE(list4.at(i), list3.at(i));
}

void SettingsTest::testByteArraySettings()
{
	AutoTestSettings settings(true);

	// Test empty array
	QCOMPARE(0, settings.byteArrayTest1().size());

	// Test changing array settings
	QByteArray array1;
	array1.append('t');
	array1.append('e');
	array1.append('s');
	array1.append('t');

	settings.setByteArrayTest2(array1);

	QByteArray array2 = settings.byteArrayTest2();

	QCOMPARE(array2.size(), array1.size());

	for(int i=0; i< array1.size();i++)
		QCOMPARE(array2.at(i), array1.at(i));
}
