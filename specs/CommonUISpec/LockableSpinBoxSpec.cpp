/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhLockableSpinBox.h"

using namespace igloo;

Describe(lockable_spinbox_test) {
	static void SetUpContext() {
		PhDebug::disable();
	}

	It(enable_after_8_clicks) {
		PhLockableSpinBox spinBox;

		Assert::That(spinBox.isEnabled(), IsFalse());
		for(int i = 0; i < 7; i++)
			QTest::mouseClick(&spinBox, Qt::LeftButton);
		Assert::That(spinBox.isEnabled(), IsFalse());
		QTest::mouseClick(&spinBox, Qt::LeftButton);
		Assert::That(spinBox.isEnabled());
	}

	It(enable_if_ctrl_is_pressed) {
		PhLockableSpinBox spinBox;

		Assert::That(spinBox.isEnabled(), IsFalse());
		QTest::mouseClick(&spinBox, Qt::LeftButton);
		Assert::That(spinBox.isEnabled(), IsFalse());

		QTest::mouseClick(&spinBox, Qt::LeftButton, Qt::ControlModifier);
		Assert::That(spinBox.isEnabled());
	}
};
