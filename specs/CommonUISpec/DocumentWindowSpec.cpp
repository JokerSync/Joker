/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTextStream>
#include <QThread>
#include <QTest>
#include <QApplication>

#include "PhTools/PhDebug.h"

#include "DocumentWindowSpecSettings.h"
#include "DocumentWindowSpecWindow.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("document window", [](){
		DocumentWindowSpecSettings *settings;

		before_each([&](){
			PhDebug::disable();
			settings = new DocumentWindowSpecSettings();
			QFile f("text.txt");
			f.open(QIODevice::WriteOnly);
			QTextStream s(&f);
			s << "hello";
			f.close();
		});

		after_each([&](){
			delete settings;
		});

		it("has_current_document", [&](){
			settings->setCurrentDocument("text.txt");

			DocumentWindowSpecWindow w(settings);
			w.processArg(0, NULL);
			AssertThat(w.text().toStdString(), Equals("hello"));
		});

		it("handle_last_folder", [&](){
			DocumentWindowSpecWindow w(settings);
			w.processArg(0, NULL);

			AssertThat(settings->lastDocumentFolder(), Equals(QDir::homePath()));

			w.openDocument("text.txt");

			QString currentFolder = QDir::currentPath();
			AssertThat(settings->lastDocumentFolder(), Equals(currentFolder));
		});

//		it("reload upon modification", [&](){
//			DocumentWindowSpecWindow w(settings);

//			w.openDocument("test.txt");

//			// Modifying it externally
//			QFile f("text.txt");
//			f.open(QIODevice::Append);
//			QTextStream s(&f);
//			s << " world";
//			f.close();

//			QThread::msleep(1000);
//			QApplication::processEvents(QEventLoop::AllEvents, 10000);
//			QThread::msleep(1000);
//			QTest::qWait(1000);
//			AssertThat(w.text().toStdString(), Equals("hello world"));
//		});
	});
});
