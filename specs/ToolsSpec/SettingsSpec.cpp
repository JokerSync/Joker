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
			settings->setIntTest3(21);
			settings->resetIntTest3();
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
			settings->setUnsignedCharTest3(0xbb);
			settings->resetUnsignedCharTest3();
			AssertThat(settings->unsignedCharTest3(), Equals(0xF0));
		});

		it("handles_bool", [&](){
			settings->setBoolTest1(true);
			AssertThat(settings->boolTest1(), IsTrue());
			AssertThat(settings->boolTest2(), IsFalse());
			AssertThat(settings->boolTest3(), IsTrue());
			settings->setBoolTest3(false);
			settings->resetBoolTest3();
			AssertThat(settings->boolTest3(), IsTrue());
		});

		it("handles_float", [&](){
			settings->setFloatTest1(-1.34f);
			AssertThat(settings->floatTest1(), Equals(-1.34f));
			AssertThat(settings->floatTest2(), Equals(0.0f));
			AssertThat(settings->floatTest3(), Equals(3.14f));
			settings->setFloatTest3(0);
			settings->resetFloatTest3();
			AssertThat(settings->floatTest3(), Equals(3.14f));
		});

		it("handles_string", [&](){
			settings->setStringTest1("test setString");
			AssertThat(settings->stringTest1(), Equals("test setString"));
			AssertThat(settings->stringTest2(), Equals(""));
			AssertThat(settings->stringTest3(), Equals("stringTest default value"));
			settings->setStringTest3("another value");
			settings->resetStringTest3();
			AssertThat(settings->stringTest3(), Equals("stringTest default value"));
		});

		it("handles_string_list", [&](){
			// Test empty string list
			AssertThat(settings->stringListTest1().size(), Equals(0));

			// Test changing string list settings

			settings->setStringTest4("Check other settings are not affected by string list settings");

			settings->setStringListTest2({"a", "b", "c", "d"});

			AssertThat(settings->stringTest4(), Equals("Check other settings are not affected by string list settings"));

			AssertThat(settings->stringListTest2(), Equals(QStringList({"a", "b", "c", "d"})));

			// Test changin an existing list settings
			settings->setStringListTest2({"e", "f", "g"});

			AssertThat(settings->stringTest4(), Equals("Check other settings are not affected by string list settings"));

			AssertThat(settings->stringListTest2(), Equals(QStringList({"e", "f", "g"})));

			// This is just to make sure that changing string
			// list settings doesn't affect other settings
			settings->setIntTest1(33);

			AssertThat(settings->stringListTest3(), Equals(QStringList({"a", "b", "c"})));

			settings->setStringListTest3(QStringList({"d", "e"}));

			AssertThat(settings->stringListTest3(), Equals(QStringList({"d", "e"})));

			settings->resetStringListTest3();
			AssertThat(settings->stringListTest3(), Equals(QStringList({"a", "b", "c"})));

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
