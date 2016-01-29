/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QAction>

#include "PhTools/PhDebug.h"

#include "WindowSpecSettings.h"
#include "WindowSpecWindow.h"

#include "PhSpec.h"
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

		it("has_current_document", [&](){
			settings->setCurrentDocument("text.txt");

			WindowSpecWindow w(settings);
			w.processArg(0, NULL);
			AssertThat(w.text(), Equals("bonjour"));
		});

		it("handle_last_folder", [&](){
			WindowSpecWindow w(settings);
			w.processArg(0, NULL);

			AssertThat(settings->lastDocumentFolder(), Equals(QDir::homePath()));

			w.openDocument("text.txt");

			QString currentFolder = QDir::currentPath();
			AssertThat(settings->lastDocumentFolder(), Equals(currentFolder));
		});
	});
});
