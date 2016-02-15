/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "SettingsSpecSettings.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("settings_test", [](){
		SettingsSpecSettings *settings;

		before_each([&](){
			PhDebug::setLogMask(PHDEBUG_SPEC_MASK);
			settings = new SettingsSpecSettings();
		});

		after_each([&](){
			delete settings;
		});

		it("clears_all_settings", [&](){

			settings->setIntTest1(1);
			settings->setUnsignedCharTest1('a');
			settings->setBoolTest1(true);
			settings->setFloatTest1(0.12f);
			settings->setStringTest1("coucou");

			QStringList list;
			list.append("alpha");
			list.append("beta");
			list.append("gamma");
			settings->setStringListTest1(list);

			QByteArray array;
			array.append('a');
			array.append('b');
			array.append('c');
			array.append('b');
			settings->setByteArrayTest1(array);

			// Clear the settings
			settings->clear();

			AssertThat(settings->intTest1(), Equals(0));
			AssertThat(settings->unsignedCharTest1(), Equals(0));
			AssertThat(settings->boolTest1(), IsFalse());
			AssertThat(settings->floatTest1(), EqualsWithDelta(0.0f, 0.001f));
		AssertThat(settings->stringTest1(), Equals(""));
			AssertThat(settings->stringListTest1().count(), Equals(0));
			AssertThat(settings->byteArrayTest1().count(), Equals(0));
		});

		it("handles_int", [&](){
			settings->setIntTest1(6);
			AssertThat(settings->intTest1(), Equals(6));
			AssertThat(settings->intTest2(), Equals(0));
			AssertThat(settings->intTest3(), Equals(4));
			AssertThat(settings->intTest4(), Equals(6));
			settings->setIntTest4(8);
			AssertThat(settings->intTest1(), Equals(6));
			AssertThat(settings->intTest4(), Equals(8));
		});

		it("handles_unsigned_char", [&](){
			settings->setUnsignedCharTest1(0x11);
			AssertThat(settings->unsignedCharTest1(), Equals(0x11));
			AssertThat(settings->unsignedCharTest2(), Equals(0x0));
			AssertThat(settings->unsignedCharTest3(), Equals(0xF0));
		});

		it("handles_bool", [&](){
			settings->setBoolTest1(true);
			AssertThat(settings->boolTest1(), IsTrue());
			AssertThat(settings->boolTest2(), IsFalse());
			AssertThat(settings->boolTest3(), IsTrue());
		});

		it("handles_float", [&](){
			settings->setFloatTest1(-1.34f);
			AssertThat(settings->floatTest1(), Equals(-1.34f));
			AssertThat(settings->floatTest2(), Equals(0.0f));
			AssertThat(settings->floatTest3(), Equals(3.14f));
		});

		it("handles_string", [&](){
			settings->setStringTest1("test setString");
		AssertThat(settings->stringTest1(), Equals("test setString"));
		AssertThat(settings->stringTest2(), Equals(""));
		AssertThat(settings->stringTest3(), Equals("stringTest default value"));
		});

		it("handles_string_list", [&](){
			// Test empty string list
			AssertThat(settings->stringListTest1().size(), Equals(0));

			// Test changing string list settings

			settings->setStringTest4("Check other settings are not affected by string list settings");
			QStringList list1;
			list1.append("a");
			list1.append("b");
			list1.append("c");
			list1.append("d");

			settings->setStringListTest2(list1);

		AssertThat(settings->stringTest4(), Equals("Check other settings are not affected by string list settings"));

			QStringList list2 = settings->stringListTest2();

			AssertThat(list2.size(), Equals(list1.size()));

			for(int i = 0; i < list1.size(); i++)
				AssertThat(list2.at(i), Equals(list1.at(i)));

			// Test changin an existing list settings
			QStringList list3;
			list3.append("e");
			list3.append("f");
			list3.append("g");

			settings->setStringListTest2(list3);

		AssertThat(settings->stringTest4(), Equals("Check other settings are not affected by string list settings"));

			QStringList list4 = settings->stringListTest2();

			AssertThat(list3.size(), Equals(list4.size()));

			for(int i = 0; i < list3.size(); i++)
				AssertThat(list4.at(i), Equals(list3.at(i)));

			// This is just to make sure that changing string
			// list settings doesn't affect other settings
			settings->setIntTest1(33);

			QStringList list5 = settings->stringListTest3();

			AssertThat(list5.size(), Equals(3));
		AssertThat(list5[0], Equals("a"));
		AssertThat(list5[1], Equals("b"));
		AssertThat(list5[2], Equals("c"));

			settings->setStringListTest3(QStringList({"d", "e"}));

			QStringList list6 = settings->stringListTest3();

			AssertThat(list6.size(), Equals(2));
		AssertThat(list6[0], Equals("d"));
		AssertThat(list6[1], Equals("e"));

			// Check changing string list settings doesn't affect
			// other settings
			AssertThat(settings->intTest1(), Equals(33));
		});

		it("handles_byte_array", [&](){
			// Test empty array
			AssertThat(settings->byteArrayTest1().size(), Equals(0));

			// Test changing array settings
			QByteArray array1;
			array1.append('t');
			array1.append('e');
			array1.append('s');
			array1.append('t');

			settings->setByteArrayTest2(array1);

			QByteArray array2 = settings->byteArrayTest2();

			AssertThat(array2.size(), Equals(array1.size()));

			for(int i = 0; i < array1.size(); i++)
				AssertThat(array2.at(i), Equals(array1.at(i)));
		});
	});
});
