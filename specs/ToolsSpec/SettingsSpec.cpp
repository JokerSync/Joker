/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QMouseEvent>

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"
#include "SettingsSpecSettings.h"

using namespace igloo;

Describe(settings_test) {
	static void SetUpContext() {
		PhDebug::disable();
	}

	SettingsSpecSettings settings;

	void SetUp() {
		settings.clear();
	}

	It(clears_all_settings) {

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

		Assert::That(settings.intTest1(), Equals(0));
		Assert::That(settings.unsignedCharTest1(), Equals(0));
		Assert::That(!settings.boolTest1());
		Assert::That(settings.floatTest1(), EqualsWithDelta(0.0f, 0.001f));
		Assert::That(settings.stringTest1().toStdString(), Equals(""));
		Assert::That(settings.stringListTest1().count(), Equals(0));
		Assert::That(settings.byteArrayTest1().count(), Equals(0));
	}

	It(handles_int) {
		settings.setIntTest1(6);
		Assert::That(settings.intTest1(), Equals(6));
		Assert::That(settings.intTest2(), Equals(0));
		Assert::That(settings.intTest3(), Equals(4));
		Assert::That(settings.intTest4(), Equals(6));
		settings.setIntTest4(8);
		Assert::That(settings.intTest1(), Equals(6));
		Assert::That(settings.intTest4(), Equals(8));
	}

	It(handles_unsigned_char) {
		settings.setUnsignedCharTest1(0x11);
		Assert::That((int)settings.unsignedCharTest1(), Equals(0x11));
		Assert::That((int)settings.unsignedCharTest2(), Equals(0x0));
		Assert::That((int)settings.unsignedCharTest3(), Equals(0xF0));
	}

	It(handles_bool) {
		settings.setBoolTest1(true);
		Assert::That(settings.boolTest1());
		Assert::That(!settings.boolTest2());
		Assert::That(settings.boolTest3());
	}

	It(handles_float) {
		settings.setFloatTest1(-1.34f);
		Assert::That(settings.floatTest1(), Equals(-1.34f));
		Assert::That(settings.floatTest2(), Equals(0.0f));
		Assert::That(settings.floatTest3(), Equals(3.14f));
	}

	It(handles_string) {
		settings.setStringTest1("test setString");
		Assert::That(settings.stringTest1().toStdString(), Equals("test setString"));
		Assert::That(settings.stringTest2().toStdString(), Equals(""));
		Assert::That(settings.stringTest3().toStdString(), Equals("stringTest default value"));
	}

	It(handles_string_list) {
		// Test empty string list
		Assert::That(settings.stringListTest1().size(), Equals(0));

		// Test changing string list settings

		settings.setStringTest4("Check other settings are not affected by string list settings");
		QStringList list1;
		list1.append("a");
		list1.append("b");
		list1.append("c");
		list1.append("d");

		settings.setStringListTest2(list1);

		Assert::That(settings.stringTest4().toStdString(), Equals("Check other settings are not affected by string list settings"));

		QStringList list2 = settings.stringListTest2();

		Assert::That(list2.size(), Equals(list1.size()));

		for(int i = 0; i < list1.size(); i++)
			Assert::That(list2.at(i), Equals(list1.at(i)));

		// Test changin an existing list settings
		QStringList list3;
		list3.append("e");
		list3.append("f");
		list3.append("g");

		settings.setStringListTest2(list3);

		Assert::That(settings.stringTest4().toStdString(), Equals("Check other settings are not affected by string list settings"));

		QStringList list4 = settings.stringListTest2();

		Assert::That(list3.size(), Equals(list4.size()));

		for(int i = 0; i < list3.size(); i++)
			Assert::That(list4.at(i), Equals(list3.at(i)));

		// This is just to make sure that changing string
		// list settings doesn't affect other settings
		settings.setIntTest1(33);

		QStringList list5 = settings.stringListTest3();

		Assert::That(list5.size(), Equals(3));
		Assert::That(list5[0].toStdString(), Equals("a"));
		Assert::That(list5[1].toStdString(), Equals("b"));
		Assert::That(list5[2].toStdString(), Equals("c"));

		settings.setStringListTest3(QStringList({"d", "e"}));

		QStringList list6 = settings.stringListTest3();

		Assert::That(list6.size(), Equals(2));
		Assert::That(list6[0].toStdString(), Equals("d"));
		Assert::That(list6[1].toStdString(), Equals("e"));

		// Check changing string list settings doesn't affect
		// other settings
		Assert::That(settings.intTest1(), Equals(33));
	}

	It(handles_byte_array) {
		// Test empty array
		Assert::That(settings.byteArrayTest1().size(), Equals(0));

		// Test changing array settings
		QByteArray array1;
		array1.append('t');
		array1.append('e');
		array1.append('s');
		array1.append('t');

		settings.setByteArrayTest2(array1);

		QByteArray array2 = settings.byteArrayTest2();

		Assert::That(array2.size(), Equals(array1.size()));

		for(int i = 0; i < array1.size(); i++)
			Assert::That(array2.at(i), Equals(array1.at(i)));
	}
};

