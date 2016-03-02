/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhMediaPanel.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("media panel", []() {
		PhMediaPanel *panel;
		PhClock *clock;

		before_each([&]() {
			PhDebug::disable();

			panel = new PhMediaPanel();
			clock = new PhClock();
			panel->setTimeCodeType(PhTimeCodeType25);
			panel->setClock(clock);
			panel->show();
		});

		after_each([&]() {
			panel->close();
			delete clock;
			delete panel;
		});

		it("handles clock", [&](){
			AssertThat(panel->isPlaying(), IsFalse());

			clock->setRate(1);
			AssertThat(panel->isPlaying(), IsTrue());

			clock->setRate(0);
			AssertThat(panel->isPlaying(), IsFalse());
		});

		it("play upon space bar pressed", [&]() {
			QTest::keyPress(panel, Qt::Key_Space);

			QApplication::processEvents();

			AssertThat(clock->rate(), Equals(1));
		});
	});
});
