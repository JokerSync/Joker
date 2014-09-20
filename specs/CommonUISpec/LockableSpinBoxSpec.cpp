/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "bandit/bandit.h"

#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhLockableSpinBox.h"

using namespace bandit;

go_bandit([](){
	describe("lockable_spinbox_test", []() {
		before_each([&](){
			PhDebug::disable();
		});

		it("enable_after_8_clicks", [&](){
			PhLockableSpinBox spinBox;

			Assert::That(spinBox.isEnabled(), IsFalse());
			for(int i = 0; i < 7; i++)
				QTest::mouseClick(&spinBox, Qt::LeftButton);
			Assert::That(spinBox.isEnabled(), IsFalse());
			QTest::mouseClick(&spinBox, Qt::LeftButton);
			Assert::That(spinBox.isEnabled());
		});

		it("enable_if_ctrl_is_pressed", [&](){
			PhLockableSpinBox spinBox;

			Assert::That(spinBox.isEnabled(), IsFalse());
			QTest::mouseClick(&spinBox, Qt::LeftButton);
			Assert::That(spinBox.isEnabled(), IsFalse());

			QTest::mouseClick(&spinBox, Qt::LeftButton, Qt::ControlModifier);
			Assert::That(spinBox.isEnabled());
		});
	});
});
