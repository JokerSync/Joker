/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhMediaPanel.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("media_panel_test", []() {
		before_each([&](){
			PhDebug::disable();
		});

		it("set_clock", [&](){
			PhMediaPanel panel;
			PhClock clock;

			panel.setTimeCodeType(PhTimeCodeType25);
			panel.setClock(&clock);

			clock.setRate(1);

			clock.elapse(600);
			clock.elapse(600);
			clock.elapse(600);
			clock.elapse(600);
			clock.elapse(600);
		});

		it("play_upon_space_bar_pressed", []() {
			PhMediaPanel panel;
			PhClock clock;

			panel.setTimeCodeType(PhTimeCodeType25);
			panel.setClock(&clock);

			QTest::keyPress(&panel, Qt::Key_Space);

			QApplication::processEvents();

			AssertThat(clock.rate(), Equals(1));
		});
	});
});
