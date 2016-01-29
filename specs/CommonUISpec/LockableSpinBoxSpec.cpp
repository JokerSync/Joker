/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhLockableSpinBox.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("lockable_spinbox_test", []() {
		before_each([&](){
			PhDebug::disable();
		});

		it("enable_after_8_clicks", [&](){
			PhLockableSpinBox spinBox;

			AssertThat(spinBox.isEnabled(), IsFalse());
			for(int i = 0; i < 7; i++)
				QTest::mouseClick(&spinBox, Qt::LeftButton);
			AssertThat(spinBox.isEnabled(), IsFalse());
			QTest::mouseClick(&spinBox, Qt::LeftButton);
			AssertThat(spinBox.isEnabled(), IsTrue());
		});

		it("enable_if_ctrl_is_pressed", [&](){
			PhLockableSpinBox spinBox;

			AssertThat(spinBox.isEnabled(), IsFalse());
			QTest::mouseClick(&spinBox, Qt::LeftButton);
			AssertThat(spinBox.isEnabled(), IsFalse());

			QTest::mouseClick(&spinBox, Qt::LeftButton, Qt::ControlModifier);
			AssertThat(spinBox.isEnabled(), IsTrue());
		});
	});
});
