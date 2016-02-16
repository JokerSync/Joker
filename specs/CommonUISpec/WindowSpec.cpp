/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QAction>

#include "PhTools/PhDebug.h"

#include "WindowSpecSettings.h"
#include "WindowSpecWindow.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("window_test", [](){
		WindowSpecSettings *settings;
		before_each([&](){
			PhDebug::disable();
			settings = new WindowSpecSettings();
		});

		after_each([&](){
			delete settings;
		});

		it("go_fullscreen_01", [&](){
			WindowSpecWindow w(settings);
			w.show();
			AssertThat(w.isFullScreen(), IsFalse());
			AssertThat(w.fullScreenAction()->isChecked(), IsFalse());

			QTest::qWait(1000);

			w.toggleFullScreen();
			AssertThat(w.isFullScreen(), IsTrue());
			AssertThat(settings->fullScreen(), IsTrue());
			AssertThat(w.fullScreenAction()->isChecked(), IsTrue());

			QTest::qWait(2000);

			w.toggleFullScreen();

			AssertThat(w.isFullScreen(), IsFalse());
			AssertThat(settings->fullScreen(), IsFalse());
			AssertThat(w.fullScreenAction()->isChecked(), IsFalse());

			QTest::qWait(1000);

			w.close();
		});

		it("go_fullscreen_02", [&](){
			settings->setFullScreen(true);
			WindowSpecWindow w(settings);
			w.show();
			QTest::qWait(3000);
			AssertThat(w.isFullScreen(), IsTrue());
			AssertThat(w.fullScreenAction()->isChecked(), IsTrue());

			w.fullScreenAction()->trigger();

			QTest::qWait(1000);
			AssertThat(w.isFullScreen(), IsFalse());
			AssertThat(w.fullScreenAction()->isChecked(), IsFalse());

			w.close();
		});

		it("recall last geometry", [&](){
			WindowSpecWindow w1(settings);
			w1.show();

			AssertThat(w1.width(), Equals(400));
			AssertThat(w1.height(), Equals(300));

			w1.setGeometry(50, 100, 600, 450);

			AssertThat(w1.width(), Equals(600));
			AssertThat(w1.height(), Equals(450));

			w1.close();

			WindowSpecWindow w2(settings);

			w2.show();

			AssertThat(w2.width(), Equals(600));
			AssertThat(w2.height(), Equals(450));
		});
	});
});
