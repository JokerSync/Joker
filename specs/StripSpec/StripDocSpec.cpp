/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhStrip/PhStripDoc.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("strip test", [&]() {
		PhStripDoc doc;

		before_each([&](){
			//PhDebug::disable();
			doc.reset();
		});

		describe("joker", [&]() {
			it("open strip, old joker file", [&](){
				AssertThat(doc.openStripFile("test.strip"), IsTrue());
				AssertThat(doc.forceRatio169(), IsTrue());
				AssertThat(doc.videoFilePath(), Equals("test01.mov"));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:59:00:00"));
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:02:03:04"));
			});

			it("open joker file linking to a detx file", [&](){
				AssertThat(doc.openStripFile("test01.joker"), IsTrue());
				AssertThat(doc.filePath(), Equals("test01.detx"));
				AssertThat(doc.videoFilePath(), Equals("test01.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:01:00:00"));
				AssertThat(doc.forceRatio169(), IsTrue());
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:30:00:00"));

				AssertThat(doc.title(), Equals("Title test"));
				AssertThat(doc.generator(), Equals("Cappella v0.12.5, 1"));
			});

			it("open joker file linking to a detx file", [&](){
				AssertThat(doc.openStripFile("test02.joker"), IsTrue());
				AssertThat(doc.filePath(), Equals("test03.mos"));
				AssertThat(doc.videoFilePath(), Equals("test02.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:03:00:00"));
				AssertThat(doc.forceRatio169(), IsFalse());
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:20:00:00"));

				AssertThat(doc.title(), Equals("Au fil d'Ariane"));
				AssertThat(doc.generator()
						   , Equals("Mosaic"));
			});

			it("fails to open bad tag joker file", [&](){
				AssertThat(QFile::exists("bad_tag.joker"), IsTrue());
				AssertThat(doc.openStripFile("bad_tag.joker"), IsFalse());

				AssertThat(QFile::exists("empty.joker"), IsTrue());
				AssertThat(doc.openStripFile("empty.joker"), IsFalse());
			});

			it("can open well formed empty file", [&](){
				AssertThat(QFile::exists("empty_root.joker"), IsTrue());
				AssertThat(doc.openStripFile("empty_root.joker"), IsTrue());

				AssertThat(QFile::exists("empty_meta.joker"), IsTrue());
				AssertThat(doc.openStripFile("empty_meta.joker"), IsTrue());
			});

			#warning /// @todo Test video frame rate

			it("open and save with detx", [&](){
				AssertThat(doc.openStripFile("test01.joker"), IsTrue());

				AssertThat(doc.saveStripFile("save01.joker", s2t("01:01:01:01", PhTimeCodeType25)), IsTrue());

				doc.reset();

				AssertThat(doc.openStripFile("save01.joker"), IsTrue());

				AssertThat(doc.filePath(), Equals("test01.detx"));
				AssertThat(doc.videoFilePath(), Equals("test01.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:01:00:00"));
				AssertThat(doc.forceRatio169(), IsTrue());
				AssertThat(doc.videoDeinterlace(), IsFalse());
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:01:01:01"));

				AssertThat(doc.title(), Equals("Title test"));
				AssertThat(doc.generator(), Equals("Cappella v0.12.5, 1"));
			});

			it("open and save with mos", [&](){
				AssertThat(doc.openStripFile("test02.joker"), IsTrue());

				AssertThat(doc.saveStripFile("save02.joker", s2t("02:02:02:02", PhTimeCodeType25)), IsTrue());

				AssertThat(doc.openStripFile("save02.joker"), IsTrue());

				AssertThat(doc.filePath(), Equals("test03.mos"));
				AssertThat(doc.videoFilePath(), Equals("test02.mov"));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:03:00:00"));
				AssertThat(doc.forceRatio169(), IsFalse());
				AssertThat(doc.videoDeinterlace(), IsTrue());
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("02:02:02:02"));

				AssertThat(doc.title(), Equals("Au fil d'Ariane"));
				AssertThat(doc.generator(), Equals("Mosaic"));
			});
		});

		describe("detx", [&]() {
			describe("import test01.detx", [&]() {
				before_each([&](){
					AssertThat(doc.importDetXFile("test01.detx"), IsTrue());
				});

				it("import header", [&](){
					AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:00:16:00"));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:00:00:00"));
				});

				it("import texts", [&](){
					AssertThat(doc.texts().count(), Equals(6));

					AssertThat(doc.texts()[0]->content(), Equals("Simple sentence"));
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:02:00"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:04:00"));
					AssertThat(doc.texts()[0]->people(), Equals(doc.peopleByName("Jeanne")));
					AssertThat(doc.texts()[0]->y(), Equals(0.25f));

					AssertThat(doc.texts()[1]->content(), Equals("Composed "));
					AssertThat(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"));
					AssertThat(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

					AssertThat(doc.texts()[2]->content(), Equals("sentence"));
					AssertThat(t2s(doc.texts()[2]->timeIn(), PhTimeCodeType25), Equals("01:00:06:00"));
					AssertThat(t2s(doc.texts()[2]->timeOut(), PhTimeCodeType25), Equals("01:00:07:00"));

					AssertThat(doc.texts()[3]->content(), Equals("Simple off sentence"));
					AssertThat(t2s(doc.texts()[3]->timeIn(), PhTimeCodeType25), Equals("01:00:12:00"));
					AssertThat(t2s(doc.texts()[3]->timeOut(), PhTimeCodeType25), Equals("01:00:14:00"));

					AssertThat(doc.texts()[4]->content(), Equals("Composed sentence with off"));
					AssertThat(t2s(doc.texts()[4]->timeIn(), PhTimeCodeType25), Equals("01:00:15:00"));
					AssertThat(t2s(doc.texts()[4]->timeOut(), PhTimeCodeType25), Equals("01:00:17:00"));

					AssertThat(doc.texts()[5]->content(), Equals("Sentence with out not linked"));
					AssertThat(t2s(doc.texts()[5]->timeIn(), PhTimeCodeType25), Equals("01:00:30:00"));
					AssertThat(t2s(doc.texts()[5]->timeOut(), PhTimeCodeType25), Equals("01:00:31:04"));
				});

				it("import detects", [&](){
					AssertThat(doc.detects().count(), Equals(6));

					AssertThat(doc.detects()[0]->people(), Equals(doc.peopleByName("Jeanne")));
					AssertThat(doc.detects()[0]->type(), Equals(PhStripDetect::On));
					AssertThat(t2s(doc.detects()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:02:00"));
					AssertThat(t2s(doc.detects()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:04:00"));
					AssertThat(doc.detects()[0]->y(), Equals(0.25f));

					AssertThat(doc.detects()[1]->people(), Equals(doc.peopleByName("Sue")));
					AssertThat(doc.detects()[1]->type(), Equals(PhStripDetect::On));
					AssertThat(t2s(doc.detects()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"));
					AssertThat(t2s(doc.detects()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:07:00"));
					AssertThat(doc.detects()[1]->y(), Equals(0.5f));


					AssertThat(doc.detects()[2]->people(), Equals(doc.peopleByName("Paul")));
					AssertThat(doc.detects()[2]->type(), Equals(PhStripDetect::Off));
					AssertThat(t2s(doc.detects()[2]->timeIn(), PhTimeCodeType25), Equals("01:00:12:00"));
					AssertThat(t2s(doc.detects()[2]->timeOut(), PhTimeCodeType25), Equals("01:00:14:00"));
					AssertThat(doc.detects()[2]->y(), Equals(0.25f));


					AssertThat(doc.detects()[3]->people(), Equals(doc.peopleByName("Sue")));
					AssertThat(doc.detects()[3]->type(), Equals(PhStripDetect::Off));
					AssertThat(t2s(doc.detects()[3]->timeIn(), PhTimeCodeType25), Equals("01:00:15:00"));
					AssertThat(t2s(doc.detects()[3]->timeOut(), PhTimeCodeType25), Equals("01:00:17:00"));
					AssertThat(doc.detects()[3]->y(), Equals(0.5f));

					AssertThat(doc.detects()[4]->people(), Equals(doc.peopleByName("Sue")));
					AssertThat(doc.detects()[4]->type(), Equals(PhStripDetect::Off));
					AssertThat(t2s(doc.detects()[4]->timeIn(), PhTimeCodeType25), Equals("01:00:20:00"));
					AssertThat(t2s(doc.detects()[4]->timeOut(), PhTimeCodeType25), Equals("01:00:22:00"));
					AssertThat(doc.detects()[4]->y(), Equals(0.5f));

					AssertThat(doc.detects()[5]->people(), Equals(doc.peopleByName("Sue")));
					AssertThat(doc.detects()[5]->type(), Equals(PhStripDetect::On));
					AssertThat(t2s(doc.detects()[5]->timeIn(), PhTimeCodeType25), Equals("01:00:30:00"));
					AssertThat(t2s(doc.detects()[5]->timeOut(), PhTimeCodeType25), Equals("01:00:31:04"));
					AssertThat(doc.detects()[5]->y(), Equals(0.5f));
				});

				it("import sentences", [&](){
					AssertThat(doc.sentences().count(), Equals(5));

					AssertThat(doc.sentences()[0]->people(), Equals(doc.peopleByName("Jeanne")));
					AssertThat(doc.sentences()[0]->content(), Equals("Simple sentence"));
					AssertThat(t2s(doc.sentences()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:02:00"));
					AssertThat(t2s(doc.sentences()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:04:00"));
					AssertThat(doc.sentences()[1]->y(), Equals(0.5f));

					AssertThat(doc.sentences()[1]->people(), Equals(doc.peopleByName("Sue")));
					AssertThat(doc.sentences()[1]->content(), Equals("Composed sentence"));
					AssertThat(t2s(doc.sentences()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"));
					AssertThat(t2s(doc.sentences()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:07:00"));
					AssertThat(doc.sentences()[1]->y(), Equals(0.5f));

					AssertThat(doc.sentences()[2]->people(), Equals(doc.peopleByName("Paul")));
					AssertThat(doc.sentences()[2]->content(), Equals("Simple off sentence"));
					AssertThat(t2s(doc.sentences()[2]->timeIn(), PhTimeCodeType25), Equals("01:00:12:00"));
					AssertThat(t2s(doc.sentences()[2]->timeOut(), PhTimeCodeType25), Equals("01:00:14:00"));
					AssertThat(doc.sentences()[2]->y(), Equals(0.25f));


					AssertThat(doc.sentences()[3]->people(), Equals(doc.peopleByName("Sue")));
					AssertThat(doc.sentences()[3]->content(), Equals("Composed sentence with off"));
					AssertThat(t2s(doc.sentences()[3]->timeIn(), PhTimeCodeType25), Equals("01:00:15:00"));
					AssertThat(t2s(doc.sentences()[3]->timeOut(), PhTimeCodeType25), Equals("01:00:17:00"));
					AssertThat(doc.sentences()[3]->y(), Equals(0.5f));

					AssertThat(doc.sentences()[4]->people(), Equals(doc.peopleByName("Sue")));
					AssertThat(doc.sentences()[4]->content(), Equals("Sentence with out not linked"));
					AssertThat(t2s(doc.sentences()[4]->timeIn(), PhTimeCodeType25), Equals("01:00:30:00"));
					AssertThat(t2s(doc.sentences()[4]->timeOut(), PhTimeCodeType25), Equals("01:00:31:04"));
					AssertThat(doc.sentences()[4]->y(), Equals(0.5f));
				});
			});

			describe("empty doc", [&]() {
				before_each([&](){
					doc.reset();
				});

				it("compute detx id", [&](){
					AssertThat(doc.computeDetXId("abc"), Equals("abc"));
					AssertThat(doc.computeDetXId("ABC"), Equals("abc"));
					AssertThat(doc.computeDetXId("It's ok!"), Equals("it_s_ok_"));
				});

				it("import detx without title", [&](){
					AssertThat(doc.importDetXFile("notitle.detx"), IsTrue());
					AssertThat(doc.title(), Equals("notitle"));
				});

				it("export and import basic detx", [&](){
					doc.setTitle("Title test for detx");
					doc.setVideoFilePath("test01.mov");
					doc.setVideoTimeIn(s2t25("01:01:00:00"), PhTimeCodeType25);
					doc.setForceRatio169(true);

					PhPeople *bob = new PhPeople("Bob", "#0000ff", "not base64 data, just for test");
					doc.addPeople(bob);
					PhPeople *sue = new PhPeople("Sue", "#ff00ff", "bouboubou");
					doc.addPeople(sue);

					doc.addLoop(new PhStripLoop(s2t25("01:01:00:00"), "1"));

					doc.addCut(new PhStripCut(s2t25("01:01:00:02"), PhStripCut::Simple));

					doc.addText(new PhStripText(s2t25("01:01:00:05"), sue, s2t25("01:01:00:15"), 0.50f, "Hello", 0.25f));

					doc.addDetect(new PhStripDetect(PhStripDetect::Off, s2t25("01:01:00:05"), sue, s2t25("01:01:00:15"), 0.50f, 0.25f));

					AssertThat(doc.exportDetXFile("save01.detx", s2t25("01:01:01:01")), IsTrue());

					doc.reset();

					AssertThat(doc.importDetXFile("save01.detx"), IsTrue());

					AssertThat(doc.filePath(), Equals("save01.detx"));
					AssertThat(doc.title(), Equals("Title test for detx"));
					AssertThat(doc.videoFilePath(), Equals("test01.mov"));
					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
					AssertThat(t2s25(doc.videoTimeIn()), Equals("01:01:00:00"));
					AssertThat(doc.forceRatio169(), IsTrue());
					AssertThat(doc.videoDeinterlace(), IsFalse());
					AssertThat(t2s25(doc.lastTime()), Equals("01:01:01:01"));

					// Peoples
					AssertThat(doc.peoples().count(), Equals(2));

					AssertThat(doc.peoples().at(0)->name(), Equals("Bob"));
					AssertThat(doc.peoples().at(0)->color(), Equals("#0000ff"));
					AssertThat(doc.peoples().at(0)->picture(), Equals("not base64 data, just for test"));

					AssertThat(doc.peoples().at(1)->name(), Equals("Sue"));
					AssertThat(doc.peoples().at(1)->color(), Equals("#ff00ff"));
					AssertThat(doc.peoples().at(1)->picture(), Equals("bouboubou"));

					// Loops
					AssertThat(doc.loops().count(), Equals(1));

					PhStripLoop *loop = doc.loops().at(0);
					AssertThat(t2s25(loop->timeIn()), Equals("01:01:00:00"));
					AssertThat(loop->label(), Equals("1"));

					// Cuts
					AssertThat(doc.cuts().count(), Equals(1));

					PhStripCut *cut = doc.cuts().at(0);
					AssertThat(t2s25(cut->timeIn()), Equals("01:01:00:02"));
					AssertThat(cut->type(), Equals(PhStripCut::Simple));

					// Texts
					AssertThat(doc.texts().count(), Equals(1));

					// simple text
					PhStripText *text = doc.texts().at(0);
					AssertThat(t2s25(text->timeIn()), Equals("01:01:00:05"));
					AssertThat(text->people()->name(), Equals("Sue"));
					AssertThat(t2s25(text->timeOut()), Equals("01:01:00:15"));
					AssertThat(text->content(), Equals("Hello"));

					// Detects
					AssertThat(doc.detects().count(), Equals(1));

					PhStripDetect *detect = doc.detects().at(0);
					AssertThat(t2s25(detect->timeIn()), Equals("01:01:00:05"));
					AssertThat(detect->people()->name(), Equals("Sue"));
					AssertThat(t2s25(detect->timeOut()), Equals("01:01:00:15"));
					AssertThat(detect->type(), Equals(PhStripDetect::Off));
				});

				it("export and import detx with complex lines", [&](){
					doc.setTitle("export and import detx with complex lines");
					PhPeople *sue = new PhPeople("Sue", "#ff00ff", "bouboubou");
					doc.addPeople(sue);

					doc.addText(new PhStripText(s2t25("01:01:00:05"), sue, s2t25("01:01:00:15"), 0.50f, "Hello ", 0.25f));
					doc.addText(new PhStripText(s2t25("01:01:00:15"), sue, s2t25("01:01:01:00"), 0.50f, "world", 0.25f));

					AssertThat(doc.exportDetXFile("save02.detx", s2t25("01:01:01:01")), IsTrue());

					doc.reset();

					AssertThat(doc.importDetXFile("save02.detx"), IsTrue());

					AssertThat(doc.filePath(), Equals("save02.detx"));
					AssertThat(doc.title(), Equals("export and import detx with complex lines"));

					// Texts
					AssertThat(doc.texts().count(), Equals(2));

					// double text
					PhStripText *text = doc.texts().at(0);
					AssertThat(t2s25(text->timeIn()), Equals("01:01:00:05"));
					AssertThat(text->people()->name(), Equals("Sue"));
					AssertThat(t2s25(text->timeOut()), Equals("01:01:00:15"));
					AssertThat(text->content(), Equals("Hello "));

					text = doc.texts().at(1);
					AssertThat(t2s25(text->timeIn()), Equals("01:01:00:15"));
					AssertThat(text->people()->name(), Equals("Sue"));
					AssertThat(t2s25(text->timeOut()), Equals("01:01:01:00"));
					AssertThat(text->content(), Equals("world"));
				});

				it("export and import detx with other timecode type", [&](){
					doc.setVideoFilePath("test.mov");
					doc.setVideoTimeIn(PhTimeCode::timeFromString("01:00:00:00", PhTimeCodeType2398), PhTimeCodeType2398);

					AssertThat(doc.exportDetXFile("save03.detx", s2t("01:01:01:01", PhTimeCodeType2398)), IsTrue());

					doc.reset();

					AssertThat(doc.importDetXFile("save03.detx"), IsTrue());

					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType2398));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType2398), Equals("01:00:00:00"));
					AssertThat(t2s(doc.lastTime(), PhTimeCodeType2398), Equals("01:01:01:01"));
				});

				it("export pretty file", [&](){
					doc.setTitle("A title");
					doc.setVideoFilePath("test.mov");
					doc.setVideoTimeIn(s2t25("01:00:00:00"), PhTimeCodeType25);
					PhPeople *bob = new PhPeople("Bob", "#FF0000", "picture");
					doc.addPeople(bob);

					doc.addLoop(new PhStripLoop(s2t25("01:00:00:00"), "1"));
					doc.addCut(new PhStripCut(s2t25("01:00:01:00"), PhStripCut::Simple));

					doc.addText(new PhStripText(s2t25("01:00:02:00"), bob, s2t25("01:00:03:00"), 0.25f, "Hi!", 0.25f));

					doc.addCut(new PhStripCut(s2t25("01:00:04:00"), PhStripCut::Simple));

					doc.addText(new PhStripText(s2t25("01:00:05:00"), bob, s2t25("01:00:06:00"), 0.25f, "What's", 0.25f));
					doc.addText(new PhStripText(s2t25("01:00:06:00"), bob, s2t25("01:00:07:00"), 0.25f, "up?", 0.25f));

					AssertThat(doc.exportDetXFile("detx02save.detx", s2t("01:01:01:01", PhTimeCodeType25)), IsTrue());

					QFile expectedFile("detx02.detx");
					AssertThat(expectedFile.open(QIODevice::ReadOnly), IsTrue());
					QFile actualFile("detx02save.detx");
					AssertThat(actualFile.open(QIODevice::ReadOnly), IsTrue());
					QString expected = QTextStream(&expectedFile).readAll();
					QString actual = QTextStream(&actualFile).readAll();

					AssertThat(actual, Equals(expected));
				});
			});
		});

		describe("mos", [&]() {
			it("import_test01", [&]() {
				PhStripDoc doc;
				AssertThat(doc.importMosFile("test01.mos"), IsTrue());

				AssertThat(doc.authorName(), Equals("auteur"));

				AssertThat(doc.videoFilePath(), Equals(""));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:00:00:00"));

				AssertThat(doc.title(), Equals("Titre VO"));
				AssertThat(doc.season(), Equals("saison"));
				AssertThat(doc.episode(), Equals("episode"));

				// Test peoples
				AssertThat(doc.peoples().count(), Equals(1));
				PhPeople * people = doc.peopleByName("Nom personnage");
				AssertThat(people != NULL, IsTrue());
				AssertThat(people->name(), Equals("Nom personnage"));

				// Test cuts
				AssertThat(doc.cuts().count(), Equals(0));

				// Test loop
				AssertThat(doc.loops().count(), Equals(0));

				// Test texts
				AssertThat(doc.texts().count(), Equals(3));
				AssertThat(doc.texts()[0]->people(), Equals(people));
				AssertThat(doc.texts()[0]->content(), Equals("Ceci "));
				AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:00:00:20"));
				AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:00:01:12"));

				AssertThat(doc.texts()[1]->content(), Equals("est un"));
				AssertThat(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("00:00:01:12"));
				AssertThat(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("00:00:01:16"));

				AssertThat(doc.texts()[2]->content(), Equals(" test."));
				AssertThat(t2s(doc.texts()[2]->timeIn(), PhTimeCodeType25), Equals("00:00:01:16"));
				AssertThat(t2s(doc.texts()[2]->timeOut(), PhTimeCodeType25), Equals("00:00:02:03"));

				// Detect test
				//#warning TODO more test on detect
				AssertThat(doc.detects().count(), Equals(4));
			});

			it("import_test02", [&]() {
				PhStripDoc doc;
				AssertThat(doc.importMosFile("test02.mos"), IsTrue());

				AssertThat(doc.videoFilePath(), Equals("C:\\Users\\Matthhou\\Desktop\\Burn Notice\\710\\BurnNotice_BCI710_VOVI.mov"));
#warning TODO Matthias told me that the timestamp was in fact 00:58:00:00...
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

				// Test peoples
				AssertThat(doc.peoples().count(), Equals(2));

				PhPeople * pierre = doc.peopleByName("Pierre");
				AssertThat(pierre != NULL, IsTrue());
				AssertThat(pierre->name(), Equals("Pierre"));

				PhPeople * marie = doc.peopleByName("Marie");
				AssertThat(marie != NULL, IsTrue());
				AssertThat(marie->name(), Equals("Marie"));

				// Test cuts
				AssertThat(doc.cuts().count(), Equals(1));
				AssertThat(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

				// Test loops
				AssertThat(doc.loops().count(), Equals(1));
				AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));
				AssertThat(doc.loops()[0]->label(), Equals("1"));

				// Test texts
				AssertThat(doc.texts().count(), Equals(2));
				AssertThat(doc.texts()[0]->people(), Equals(pierre));
				AssertThat(doc.texts()[0]->y(), Equals(0.0f));
				AssertThat(doc.texts()[0]->content(), Equals("Bonjour, Marie."));
				AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));
				AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:02:00"));

				AssertThat(doc.texts()[1]->people(), Equals(marie));
				AssertThat(doc.texts()[1]->y(), Equals(0.6f));
				AssertThat(doc.texts()[1]->content(), Equals("Bonjour, Pierre."));
				AssertThat(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:04:00"));
				AssertThat(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

				// Detect test
				AssertThat(doc.detects().count(), Equals(0));
			});

			it("import_test03", [&]() {
				PhStripDoc doc;
				AssertThat(doc.importMosFile("test03.mos"), IsTrue());

				AssertThat(doc.videoFilePath(), Equals("Z:\\MOT POUR MO\\AU FIL D'ARIANE_DETECTION\\jpegAFA_BOB 06_SEQ 30_PISTES SEPARES_H264.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType24));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType24), Equals("05:59:50:00"));

				// Test peoples
				AssertThat(doc.peoples().count(), Equals(2));

				PhPeople * noName = doc.peopleByName("");
				AssertThat(noName != NULL, IsTrue());
				AssertThat(noName->name(), Equals(""));

				PhPeople * denis = doc.peopleByName("Denis");
				AssertThat(denis != NULL, IsTrue());
				AssertThat(denis->name(), Equals("Denis"));

				// Test cuts
				AssertThat(doc.cuts().count(), Equals(29));
				AssertThat(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType24), Equals("05:59:50:00"));
				AssertThat(t2s(doc.cuts()[1]->timeIn(), PhTimeCodeType24), Equals("05:59:58:00"));
				AssertThat(t2s(doc.cuts()[2]->timeIn(), PhTimeCodeType24), Equals("06:00:00:00"));
				AssertThat(t2s(doc.cuts()[3]->timeIn(), PhTimeCodeType24), Equals("06:00:05:22"));
				AssertThat(t2s(doc.cuts()[4]->timeIn(), PhTimeCodeType24), Equals("06:00:08:05"));

				// Test loops
				AssertThat(doc.loops().count(), Equals(2));
				AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType24), Equals("06:00:01:00"));
				AssertThat(doc.loops()[0]->label(), Equals("30"));
				AssertThat(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType24), Equals("06:01:15:00"));
				AssertThat(doc.loops()[1]->label(), Equals("off"));

				// Test texts
				AssertThat(doc.texts(false).count(), Equals(206));
				AssertThat(doc.texts(true).count(), Equals(0));

				AssertThat(doc.texts()[0]->people(), Equals(noName));
				AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType24), Equals("05:59:49:05"));
				AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType24), Equals("05:59:49:20"));

				AssertThat(doc.texts()[0]->y(), Equals(2.0f));
				AssertThat(doc.texts()[0]->height(), Equals(0.2f));

				AssertThat(doc.texts()[0]->content(), Equals("Départ bob 6"));
				AssertThat(doc.texts()[1]->content(), Equals("(X)"));
				AssertThat(doc.texts()[2]->content(), Equals("05.59.50.00"));
				AssertThat(doc.texts()[3]->content(), Equals("1000"));
				AssertThat(doc.texts()[4]->content(), Equals("(X)"));
				AssertThat(doc.texts()[5]->content(), Equals("P.I"));

				AssertThat(doc.texts()[9]->people(), Equals(denis));
				AssertThat(t2s(doc.texts()[9]->timeIn(), PhTimeCodeType24), Equals("06:00:07:23"));
				AssertThat(t2s(doc.texts()[9]->timeOut(), PhTimeCodeType24), Equals("06:00:08:03"));
				AssertThat(t2s(doc.texts()[10]->timeIn(), PhTimeCodeType24), Equals("06:00:08:03"));
				AssertThat(t2s(doc.texts()[10]->timeOut(), PhTimeCodeType24), Equals("06:00:08:07"));

				AssertThat(doc.texts()[9]->content(), Equals("Tu "));
				AssertThat(doc.texts()[10]->content(), Equals("tra"));
				AssertThat(doc.texts()[11]->content(), Equals("vaillais "));
				AssertThat(doc.texts()[12]->content(), Equals("pas "));
				AssertThat(doc.texts()[13]->content(), Equals("ce "));
				AssertThat(doc.texts()[14]->content(), Equals("soir !"));

				AssertThat(t2s(doc.texts()[14]->timeOut(), PhTimeCodeType24), Equals("06:00:09:06"));

				// Detect test
				AssertThat(doc.detects().count(), Equals(24));
				//#warning TODO more test on detect
			});

			it("import_test04", [&]() {
				PhStripDoc doc;

				AssertThat(doc.importMosFile("test04.mos"), IsTrue());

				// Test video info
				AssertThat(doc.videoFilePath(), Equals("D:\\Ressources\\Mosaic\\Utilisateurs\\Yves\\Bold 5704\\5704.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:59:39:24"));

				// Test loops
				AssertThat(doc.loops().count(), Equals(27));
				AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:13"));
				AssertThat(doc.loops()[0]->label(), Equals("1"));
				AssertThat(doc.loops()[1]->label(), Equals("2"));
				AssertThat(doc.loops()[2]->label(), Equals("3"));
				AssertThat(doc.loops()[3]->label(), Equals("off"));
				AssertThat(doc.loops()[4]->label(), Equals("4"));
				AssertThat(t2s(doc.loops()[5]->timeIn(), PhTimeCodeType25), Equals("01:02:56:02"));
				AssertThat(doc.loops()[5]->label(), Equals("5"));
				AssertThat(t2s(doc.loops()[25]->timeIn(), PhTimeCodeType25), Equals("01:18:14:11"));
				AssertThat(doc.loops()[25]->label(), Equals("25"));
				AssertThat(t2s(doc.loops()[26]->timeIn(), PhTimeCodeType25), Equals("01:19:01:01"));
				AssertThat(doc.loops()[26]->label(), Equals("off"));

				// Test texts
				AssertThat(doc.texts().count(), Equals(1118));
				AssertThat(doc.texts(true).count(), Equals(4329));
				AssertThat(doc.detects().count(), Equals(4552));

				AssertThat(t2s(doc.detects()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:01:13"));
				AssertThat(t2s(doc.detects()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:02:20"));
				AssertThat(doc.detects()[0]->type(), Equals(PhStripDetect::Off));
			});

			it("import_mos24", [&]() {
				PhStripDoc doc;

				AssertThat(doc.importMosFile("mos24.mos"), IsTrue());

				AssertThat(doc.videoFilePath(), Equals("C:\\Users\\Gilles\\Desktop\\Sonic_EP_01_mix_VA.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType24));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType24), Equals("00:59:58:00"));

				AssertThat(doc.texts().count(), Equals(1));

				AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType24), Equals("01:00:01:00"));
				AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType24), Equals("01:00:03:00"));
			});

			it("import_mos25", [&]() {
				PhStripDoc doc;

				AssertThat(doc.importMosFile("mos25.mos"), IsTrue());

				AssertThat(doc.videoFilePath(), Equals("C:\\Users\\Gilles\\Desktop\\Get Blake\\Get Blake 115\\GBL_EP115_Online_Master_VA_h264_TCI.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("09:59:58:00"));
			});
		});

		describe("drb", [&]() {
			describe("v5", [&]() {
				it("import_drb01", [&]() {
					AssertThat(doc.openStripFile("drb01.drb"), IsTrue());

					AssertThat(doc.videoFilePath(), Equals("C:\\Users\\SYNCHRO USER\\Downloads\\TheManWithTheGoldenArm_25fps_high\\TheManWithTheGoldenArm_25fps_high.mov"));

					AssertThat(doc.loops().count(), Equals(0));

					AssertThat(doc.peoples().count(), Equals(2));
					AssertThat(doc.peoples()[0]->name(), Equals("Personnage 1"));
					AssertThat(doc.peoples()[1]->name(), Equals("Personnage 2"));

					AssertThat(doc.texts().count(), Equals(1));
					AssertThat(doc.texts()[0]->people()->name(), Equals("Personnage 2"));

					AssertThat(doc.texts()[0]->y(), Equals(0.36f));
					AssertThat(doc.texts()[0]->height(), EqualsWithDelta(0.22666667f, 0.00001f));

#warning /// @todo should be the same than syn6
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:02:12:10"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:02:44:07"));
				});

				it("import_drb02", [&]() {
					AssertThat(doc.openStripFile("drb02.drb"), IsTrue());

					AssertThat(doc.videoFilePath(), Equals("D:\\NED 201.mov"));
					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:04:20"));

					AssertThat(doc.loops().count(), Equals(21));

					for(int i = 0; i < 21; i++)
						AssertThat(doc.loops().at(i)->label(), Equals(QString::number(i + 1)));

					AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:39:02"));
					AssertThat(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType25), Equals("01:02:14:23"));

					AssertThat(doc.peoples().count(), Equals(28));

					AssertThat(doc.peoples()[0]->name(), Equals("Intervenant 1"));
					AssertThat(doc.peoples()[1]->name(), Equals("ned"));
					AssertThat(doc.peoples()[2]->name(), Equals("moze"));
					AssertThat(doc.peoples()[3]->name(), Equals("suzie"));

					AssertThat(doc.texts().count(), Equals(546));
					AssertThat(doc.texts()[0]->people()->name(), Equals("ned"));
					AssertThat(doc.texts()[0]->y(), Equals(0.213333338f));
					AssertThat(doc.texts()[0]->height(), Equals(0.28666667f));
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:13"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:01:05"));
				});

				it("import_drb03", [&]() {
					AssertThat(doc.openStripFile("drb03.drb"), IsTrue());

					AssertThat(doc.videoFilePath(), Equals("C:\\Users\\Matthhou\\Desktop\\The Crazy Ones\\The Crazy Ones 121\\The_Crazy_Ones_1AXB21_VOVI.mov"));
					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

					AssertThat(doc.loops().count(), Equals(1));

					AssertThat(doc.loops().at(0)->label(), Equals("1"));

					AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

					AssertThat(doc.peoples().count(), Equals(2));

					AssertThat(doc.peoples()[0]->name(), Equals("Pierre"));
					AssertThat(doc.peoples()[1]->name(), Equals("Marie"));

					AssertThat(doc.texts().count(), Equals(2));

					AssertThat(doc.texts()[0]->people()->name(), Equals("Pierre"));
					AssertThat(doc.texts()[0]->y(), Equals(0.066666667f));
					AssertThat(doc.texts()[0]->height(), Equals(0.22666667f));
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:01:00"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:03:00"));

					AssertThat(doc.texts()[1]->people()->name(), Equals("Marie"));
					AssertThat(doc.texts()[1]->y(), Equals(0.7f));
					AssertThat(doc.texts()[1]->height(), Equals(0.22666667f));
					AssertThat(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:04:00"));
					AssertThat(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

					AssertThat(doc.cuts().count(), Equals(1));
					AssertThat(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"));
				});
			});

			describe("v6", [&]() {
				it("import_test01", [&]() {
					AssertThat(doc.openStripFile("test01.syn6"), IsTrue());

					AssertThat(doc.peoples().count(), Equals(2));
					AssertThat(doc.peoples()[0]->name(), Equals("Personnage 1"));
					AssertThat(doc.peoples()[1]->name(), Equals("Personnage 2"));

					AssertThat(doc.texts().count(), Equals(1));
					AssertThat(doc.texts()[0]->people()->name(), Equals("Personnage 1"));

					AssertThat(doc.texts()[0]->y(), Equals(0.36f));
					AssertThat(doc.texts()[0]->height(), Equals(0.22666667f));

					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:00:03:11"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:00:06:14"));
				});

				it("import_test02", [&]() {
					AssertThat(doc.openStripFile("test02.syn6"), IsTrue());

					AssertThat(doc.videoFilePath(), Equals("C:\\Users\\Matthhou\\Desktop\\The Crazy Ones\\The Crazy Ones 121\\The_Crazy_Ones_1AXB21_VOVI.mov"));
					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

					AssertThat(doc.loops().count(), Equals(1));

					AssertThat(doc.loops().at(0)->label(), Equals("1"));

					AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

					AssertThat(doc.peoples().count(), Equals(2));

					AssertThat(doc.peoples()[0]->name(), Equals("Pierre"));
					AssertThat(doc.peoples()[1]->name(), Equals("Marie"));

					AssertThat(doc.texts().count(), Equals(2));

					AssertThat(doc.texts()[0]->people()->name(), Equals("Pierre"));
					AssertThat(doc.texts()[0]->y(), Equals(0.066666667f));
					AssertThat(doc.texts()[0]->height(), Equals(0.22666667f));
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:01:00"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:03:00"));

					AssertThat(doc.texts()[1]->people()->name(), Equals("Marie"));
					AssertThat(doc.texts()[1]->y(), Equals(0.7f));
					AssertThat(doc.texts()[1]->height(), Equals(0.22666667f));
					AssertThat(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:04:00"));
					AssertThat(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

					AssertThat(doc.cuts().count(), Equals(1));
					AssertThat(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"));
				});
			});

		});

		it("get people by name", [&](){
			AssertThat(doc.importDetXFile("test01.detx"), IsTrue());

			AssertThat(doc.peopleByName("Jeanne")->name(), Equals("Jeanne"));
			AssertThat(doc.peopleByName("Sue")->name(), Equals("Sue"));
			AssertThat(doc.peopleByName("Paul")->name(), Equals("Paul"));
			AssertThat(doc.peopleByName("Bob") == NULL, IsTrue());
		});

		it("get previous element time", [&](){
			AssertThat(doc.importDetXFile("test01.detx"), IsTrue());
			PhTimeCodeType tcType = PhTimeCodeType25;

			AssertThat(t2s(doc.previousElementTime(s2t("23:00:00:00", tcType)), tcType), Equals("01:01:00:00"));
			AssertThat(t2s(doc.previousElementTime(s2t("01:01:00:00", tcType)), tcType), Equals("01:00:30:00"));
			AssertThat(t2s(doc.previousElementTime(s2t("01:00:30:00", tcType)), tcType), Equals("01:00:15:00"));
			AssertThat(t2s(doc.previousElementTime(s2t("01:00:15:00", tcType)), tcType), Equals("01:00:12:00"));
			AssertThat(doc.previousElementTime(s2t("01:00:00:00", tcType)), Equals(PHTIMEMIN));
		});

		it("get next element time", [&](){
			AssertThat(doc.importDetXFile("test01.detx"), IsTrue());
			PhTimeCodeType tcType = PhTimeCodeType25;

			AssertThat(t2s(doc.nextElementTime(s2t("00:00:00:00", tcType)), tcType), Equals("01:00:00:00"));
			AssertThat(t2s(doc.nextElementTime(s2t("01:00:00:00", tcType)), tcType), Equals("01:00:01:00"));
			AssertThat(t2s(doc.nextElementTime(s2t("01:00:01:00", tcType)), tcType), Equals("01:00:02:00"));
			AssertThat(t2s(doc.nextElementTime(s2t("01:00:02:00", tcType)), tcType), Equals("01:00:05:00"));
			AssertThat(t2s(doc.nextElementTime(s2t("01:00:17:00", tcType)), tcType), Equals("01:00:30:00"));
			AssertThat(t2s(doc.nextElementTime(s2t("01:00:30:00", tcType)), tcType), Equals("01:01:00:00"));
			AssertThat(doc.nextElementTime(s2t("01:01:00:00", tcType)), Equals(PHTIMEMAX));
		});

		it("get next text", [&](){
			AssertThat(doc.importDetXFile("test01.detx"), IsTrue());
			PhTimeCodeType tcType = PhTimeCodeType25;

			AssertThat(t2s(doc.nextText(s2t("00:00:00:00", tcType))->timeIn(), tcType), Equals("01:00:02:00"));
			AssertThat(t2s(doc.nextText(s2t("01:00:02:00", tcType))->timeIn(), tcType), Equals("01:00:05:00"));
			AssertThat(t2s(doc.nextText(s2t("01:00:05:00", tcType))->timeIn(), tcType), Equals("01:00:06:00"));
			AssertThat(t2s(doc.nextText(s2t("01:00:06:00", tcType))->timeIn(), tcType), Equals("01:00:12:00"));
			AssertThat(t2s(doc.nextText(s2t("01:00:12:00", tcType))->timeIn(), tcType), Equals("01:00:15:00"));
			AssertThat(t2s(doc.nextText(s2t("01:00:15:00", tcType))->timeIn(), tcType), Equals("01:00:30:00"));
			AssertThat(doc.nextText(s2t("01:00:30:00", tcType)) == NULL, IsTrue());
		});

		it("get next text by people", [&](){
			AssertThat(doc.importDetXFile("test01.detx"), IsTrue());
			PhTimeCodeType tcType = PhTimeCodeType25;

			PhPeople* sue = doc.peopleByName("Sue");

			AssertThat(t2s(doc.nextText(sue, s2t("00:00:00:00", tcType))->timeIn(), tcType), Equals("01:00:05:00"));
			AssertThat(t2s(doc.nextText(sue, s2t("01:00:05:00", tcType))->timeIn(), tcType), Equals("01:00:06:00"));
			AssertThat(t2s(doc.nextText(sue, s2t("01:00:06:00", tcType))->timeIn(), tcType), Equals("01:00:15:00"));
			AssertThat(t2s(doc.nextText(sue, s2t("01:00:15:00", tcType))->timeIn(), tcType), Equals("01:00:30:00"));
			AssertThat(doc.nextText(sue, s2t("01:00:30:00", tcType)) == NULL, IsTrue());
		});

		it("get next text by people list", [&](){
			AssertThat(doc.importDetXFile("test01.detx"), IsTrue());
			PhTimeCodeType tcType = PhTimeCodeType25;

			QList<PhPeople*> peopleList;
			peopleList.append(doc.peopleByName("Sue"));
			peopleList.append(doc.peopleByName("Paul"));

			AssertThat(t2s(doc.nextText(peopleList, s2t("00:00:00:00", tcType))->timeIn(), tcType), Equals("01:00:05:00"));
			AssertThat(t2s(doc.nextText(peopleList, s2t("01:00:05:00", tcType))->timeIn(), tcType), Equals("01:00:06:00"));
			AssertThat(t2s(doc.nextText(peopleList, s2t("01:00:06:00", tcType))->timeIn(), tcType), Equals("01:00:12:00"));
			AssertThat(t2s(doc.nextText(peopleList, s2t("01:00:12:00", tcType))->timeIn(), tcType), Equals("01:00:15:00"));
			AssertThat(t2s(doc.nextText(peopleList, s2t("01:00:15:00", tcType))->timeIn(), tcType), Equals("01:00:30:00"));
			AssertThat(doc.nextText(peopleList, s2t("01:00:30:00", tcType)) == NULL, IsTrue());
		});

		it("get next loop", [&](){
			AssertThat(doc.importDetXFile("test01.detx"), IsTrue());
			PhTimeCodeType tcType = PhTimeCodeType25;

			AssertThat(t2s(doc.nextLoop(s2t("00:00:00:00", tcType))->timeIn(), tcType), Equals("01:00:00:00"));
			AssertThat(t2s(doc.nextLoop(s2t("01:00:00:00", tcType))->timeIn(), tcType), Equals("01:01:00:00"));
			AssertThat(doc.nextLoop(s2t("01:01:00:00", tcType)) == NULL, IsTrue());
		});

		it("get previous loop", [&](){
			AssertThat(doc.importDetXFile("test01.detx"), IsTrue());
			PhTimeCodeType tcType = PhTimeCodeType25;

			AssertThat(doc.previousLoop(s2t("01:00:00:00", tcType)) == NULL, IsTrue());
			AssertThat(t2s(doc.previousLoop(s2t("01:01:00:00", tcType))->timeIn(), tcType), Equals("01:00:00:00"));
			AssertThat(t2s(doc.previousLoop(s2t("23:00:00:00", tcType))->timeIn(), tcType), Equals("01:01:00:00"));
		});

		it("add objects", [&](){
			doc.addPeople(new PhPeople("A people"));

			doc.addText(new PhStripText(0, doc.peoples().last(), 10000, 1, "Hello", 0.25f));
			AssertThat(doc.texts().count(), Equals(1));
			doc.addCut(new PhStripCut(5400, PhStripCut::CrossFade));
			AssertThat(doc.cuts().count(), Equals(1));
			doc.addDetect(new PhStripDetect(PhStripDetect::Aperture, 10000, doc.peoples().last(), 11000, 0.25f, 0.25f));
			AssertThat(doc.detects().count(), Equals(1));

			doc.addLoop(new PhStripLoop(22000, "3"));
			AssertThat(doc.loops().count(), Equals(1));

		});

		it("add people", [&](){
			doc.addPeople(new PhPeople("A people"));
			AssertThat(doc.peoples().count(), Equals(1));
			doc.addPeople(new PhPeople("A second people"));
			AssertThat(doc.peoples().count(), Equals(2));
		});

		it("add sentences", [&](){
			PhPeople *bob = new PhPeople("Bob");
			PhPeople *jeanne = new PhPeople("Jeanne");

			doc.addPeople(bob);
			doc.addPeople(jeanne);

			doc.addText(new PhStripText(0, bob, 10, 0.5f, "hello", 0.25f));

			AssertThat(doc.texts().count(), Equals(1));
			AssertThat(doc.sentences().count(), Equals(1));
			AssertThat(doc.sentences()[0]->timeIn(), Equals(0));
			AssertThat(doc.sentences()[0]->people(), Equals(bob));
			AssertThat(doc.sentences()[0]->timeOut(), Equals(10));
			AssertThat(doc.sentences()[0]->content(), Equals("hello"));

			doc.addText(new PhStripText(10, bob, 20, 0.5f, " world", 0.25f));

			AssertThat(doc.sentences().count(), Equals(1));
			AssertThat(doc.sentences()[0]->timeIn(), Equals(0));
			AssertThat(doc.sentences()[0]->people(), Equals(bob));
			AssertThat(doc.sentences()[0]->timeOut(), Equals(20));
			AssertThat(doc.sentences()[0]->content(), Equals("hello world"));

			doc.addText(new PhStripText(30, bob, 40, 0.5, "alright?", 0x25f));

			AssertThat(doc.sentences().count(), Equals(2));
			AssertThat(doc.sentences()[1]->timeIn(), Equals(30));
			AssertThat(doc.sentences()[1]->people(), Equals(bob));
			AssertThat(doc.sentences()[1]->timeOut(), Equals(40));
			AssertThat(doc.sentences()[1]->content(), Equals("alright?"));

			doc.addText(new PhStripText(40, jeanne, 50, 0.5, "cool!", 0x25f));

			AssertThat(doc.sentences().count(), Equals(3));
			AssertThat(doc.sentences()[2]->timeIn(), Equals(40));
			AssertThat(doc.sentences()[2]->people(), Equals(jeanne));
			AssertThat(doc.sentences()[2]->timeOut(), Equals(50));
			AssertThat(doc.sentences()[2]->content(), Equals("cool!"));

			AssertThat(doc.sentences(bob).count(), Equals(2));

			doc.reset();

			AssertThat(doc.sentences().count(), Equals(0));
		});
	});
});
