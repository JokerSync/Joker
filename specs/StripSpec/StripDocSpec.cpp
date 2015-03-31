/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhStrip/PhStripDoc.h"

#include "CommonSpec.h"

#include "PhSpec.h"

using namespace bandit;

go_bandit([](){
	describe("strip test", [&]() {
		PhStripDoc doc;

		before_each([&](){
			PhDebug::disable();
			doc.reset();
		});

		describe("joker", [&]() {
			it("open strip, old joker file", [&](){
				AssertThat(doc.openStripFile("test.strip"), IsTrue());
				AssertThat(doc.forceRatio169(), IsTrue());
				AssertThat(doc.videoFilePath().toStdString(), Equals("test01.mov"));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:59:00:00"));
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:02:03:04"));
			});

			it("open joker file linking to a detx file", [&](){
				AssertThat(doc.openStripFile("test01.joker"), IsTrue());
				AssertThat(doc.filePath().toStdString(), Equals("test01.detx"));
				AssertThat(doc.videoFilePath().toStdString(), Equals("test01.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:01:00:00"));
				AssertThat(doc.forceRatio169(), IsTrue());
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:30:00:00"));

				AssertThat(doc.title().toStdString(), Equals("Title test"));
				AssertThat(doc.generator().toStdString(), Equals("Cappella v0.12.5, 1"));
			});

			it("open joker file linking to a detx file", [&](){
				AssertThat(doc.openStripFile("test02.joker"), IsTrue());
				AssertThat(doc.filePath().toStdString(), Equals("test03.mos"));
				AssertThat(doc.videoFilePath().toStdString(), Equals("test02.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:03:00:00"));
				AssertThat(doc.forceRatio169(), IsFalse());
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:20:00:00"));

				AssertThat(doc.title().toStdString(), Equals("Au fil d'Ariane"));
				AssertThat(doc.generator().toStdString(), Equals("Mosaic"));
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

				AssertThat(doc.filePath().toStdString(), Equals("test01.detx"));
				AssertThat(doc.videoFilePath().toStdString(), Equals("test01.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:01:00:00"));
				AssertThat(doc.forceRatio169(), IsTrue());
				AssertThat(doc.videoDeinterlace(), IsFalse());
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:01:01:01"));

				AssertThat(doc.title().toStdString(), Equals("Title test"));
				AssertThat(doc.generator().toStdString(), Equals("Cappella v0.12.5, 1"));
			});

			it("open and save with mos", [&](){
				AssertThat(doc.openStripFile("test02.joker"), IsTrue());

				AssertThat(doc.saveStripFile("save02.joker", s2t("02:02:02:02", PhTimeCodeType25)), IsTrue());

				AssertThat(doc.openStripFile("save02.joker"), IsTrue());

				AssertThat(doc.filePath().toStdString(), Equals("test03.mos"));
				AssertThat(doc.videoFilePath().toStdString(), Equals("test02.mov"));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:03:00:00"));
				AssertThat(doc.forceRatio169(), IsFalse());
				AssertThat(doc.videoDeinterlace(), IsTrue());
				AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("02:02:02:02"));

				AssertThat(doc.title().toStdString(), Equals("Au fil d'Ariane"));
				AssertThat(doc.generator().toStdString(), Equals("Mosaic"));
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

				it("import_text", [&](){
					AssertThat(doc.texts().count(), Equals(6));

					AssertThat(doc.texts()[0]->content().toStdString(), Equals("Simple sentence"));
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:02:00"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:04:00"));
					AssertThat(doc.texts()[0]->people(), Equals(doc.peopleByName("Jeanne")));
					AssertThat(doc.texts()[0]->y(), Equals(0.25f));

					AssertThat(doc.texts()[1]->content().toStdString(), Equals("Composed "));
					AssertThat(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:05:00"));
					AssertThat(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

					AssertThat(doc.texts()[2]->content().toStdString(), Equals("sentence"));
					AssertThat(t2s(doc.texts()[2]->timeIn(), PhTimeCodeType25), Equals("01:00:06:00"));
					AssertThat(t2s(doc.texts()[2]->timeOut(), PhTimeCodeType25), Equals("01:00:07:00"));

					AssertThat(doc.texts()[3]->content().toStdString(), Equals("Simple off sentence"));
					AssertThat(t2s(doc.texts()[3]->timeIn(), PhTimeCodeType25), Equals("01:00:12:00"));
					AssertThat(t2s(doc.texts()[3]->timeOut(), PhTimeCodeType25), Equals("01:00:14:00"));

					AssertThat(doc.texts()[4]->content().toStdString(), Equals("Composed sentence with off"));
					AssertThat(t2s(doc.texts()[4]->timeIn(), PhTimeCodeType25), Equals("01:00:15:00"));
					AssertThat(t2s(doc.texts()[4]->timeOut(), PhTimeCodeType25), Equals("01:00:17:00"));

					AssertThat(doc.texts()[5]->content().toStdString(), Equals("Sentence with out not linked"));
					AssertThat(t2s(doc.texts()[5]->timeIn(), PhTimeCodeType25), Equals("01:00:30:00"));
					AssertThat(t2s(doc.texts()[5]->timeOut(), PhTimeCodeType25), Equals("01:00:31:04"));
				});

				it("import detect", [&](){
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
			});

			describe("import test01.detx", [&]() {
				before_each([&](){
					doc.reset();
				});

				it("import detx without title", [&](){
					AssertThat(doc.importDetXFile("notitle.detx"), IsTrue());
					AssertThat(doc.title().toStdString(), Equals("notitle"));
				});

				it("export and import detx", [&](){
					doc.setTitle("Title test for detx");
					doc.setVideoFilePath("test01.mov");
					doc.setVideoTimeIn(s2t("01:01:00:00", PhTimeCodeType25), PhTimeCodeType25);
					doc.addPeople(new PhPeople("Bob", "#0000ff", "not base64 data, just for test"));
					//doc.addPeople(new PhPeople("Sue", "#ff00ff", "bouboubou"));

					AssertThat(doc.exportDetXFile("save01.detx", s2t("01:01:01:01", PhTimeCodeType25)), IsTrue());

					doc.reset();

					AssertThat(doc.importDetXFile("save01.detx"), IsTrue());

					AssertThat(doc.filePath().toStdString(), Equals("save01.detx"));
					AssertThat(doc.title().toStdString(), Equals("Title test for detx"));
					AssertThat(doc.videoFilePath().toStdString(), Equals("test01.mov"));
					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("01:01:00:00"));
					AssertThat(doc.forceRatio169(), IsFalse());
					AssertThat(doc.videoDeinterlace(), IsFalse());
					AssertThat(t2s(doc.lastTime(), PhTimeCodeType25), Equals("01:01:01:01"));

					// test people
					AssertThat(doc.peoples().count(), Equals(1));
//					AssertThat(doc.peoples().count(), Equals(2));

					AssertThat(doc.peoples().at(0)->name().toStdString(), Equals("Bob"));
					AssertThat(doc.peoples().at(0)->color().toStdString(), Equals("#0000ff"));
//					AssertThat(doc.peoples().at(0)->picture().toStdString(), Equals("not base64 data, just for test"));

//					AssertThat(doc.peoples().at(1)->name().toStdString(), Equals("Sue"));
//					AssertThat(doc.peoples().at(1)->color().toStdString(), Equals("#ff00ff"));
//					AssertThat(doc.peoples().at(1)->picture().toStdString(), Equals("not base64 data, just for test"));
				});
			});
		});

		describe("mos", [&]() {
			it("import_test01", [&]() {
				PhStripDoc doc;
				AssertThat(doc.importMosFile("test01.mos"), IsTrue());

				AssertThat(doc.authorName().toStdString(), Equals("auteur"));

				AssertThat(doc.videoFilePath().toStdString(), Equals(""));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:00:00:00"));

				AssertThat(doc.title().toStdString(), Equals("Titre VO"));
				AssertThat(doc.season().toStdString(), Equals("saison"));
				AssertThat(doc.episode().toStdString(), Equals("episode"));

				// Test peoples
				AssertThat(doc.peoples().count(), Equals(1));
				PhPeople * people = doc.peopleByName("Nom personnage");
				AssertThat(people != NULL, IsTrue());
				AssertThat(people->name().toStdString(), Equals("Nom personnage"));

				// Test cuts
				AssertThat(doc.cuts().count(), Equals(0));

				// Test loop
				AssertThat(doc.loops().count(), Equals(0));

				// Test texts
				AssertThat(doc.texts().count(), Equals(3));
				AssertThat(doc.texts()[0]->people(), Equals(people));
				AssertThat(doc.texts()[0]->content().toStdString(), Equals("Ceci "));
				AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:00:00:20"));
				AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:00:01:12"));

				AssertThat(doc.texts()[1]->content().toStdString(), Equals("est un"));
				AssertThat(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("00:00:01:12"));
				AssertThat(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("00:00:01:16"));

				AssertThat(doc.texts()[2]->content().toStdString(), Equals(" test."));
				AssertThat(t2s(doc.texts()[2]->timeIn(), PhTimeCodeType25), Equals("00:00:01:16"));
				AssertThat(t2s(doc.texts()[2]->timeOut(), PhTimeCodeType25), Equals("00:00:02:03"));

				// Detect test
				//#warning TODO more test on detect
				AssertThat(doc.detects().count(), Equals(4));
			});

			it("import_test02", [&]() {
				PhStripDoc doc;
				AssertThat(doc.importMosFile("test02.mos"), IsTrue());

				AssertThat(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Matthhou\\Desktop\\Burn Notice\\710\\BurnNotice_BCI710_VOVI.mov"));
#warning TODO Matthias told me that the timestamp was in fact 00:58:00:00...
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

				// Test peoples
				AssertThat(doc.peoples().count(), Equals(2));

				PhPeople * pierre = doc.peopleByName("Pierre");
				AssertThat(pierre != NULL, IsTrue());
				AssertThat(pierre->name().toStdString(), Equals("Pierre"));

				PhPeople * marie = doc.peopleByName("Marie");
				AssertThat(marie != NULL, IsTrue());
				AssertThat(marie->name().toStdString(), Equals("Marie"));

				// Test cuts
				AssertThat(doc.cuts().count(), Equals(1));
				AssertThat(t2s(doc.cuts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

				// Test loops
				AssertThat(doc.loops().count(), Equals(1));
				AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));
				AssertThat(doc.loops()[0]->label().toStdString(), Equals("1"));

				// Test texts
				AssertThat(doc.texts().count(), Equals(2));
				AssertThat(doc.texts()[0]->people(), Equals(pierre));
				AssertThat(doc.texts()[0]->y(), Equals(0.0f));
				AssertThat(doc.texts()[0]->content().toStdString(), Equals("Bonjour, Marie."));
				AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));
				AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:02:00"));

				AssertThat(doc.texts()[1]->people(), Equals(marie));
				AssertThat(doc.texts()[1]->y(), Equals(0.6f));
				AssertThat(doc.texts()[1]->content().toStdString(), Equals("Bonjour, Pierre."));
				AssertThat(t2s(doc.texts()[1]->timeIn(), PhTimeCodeType25), Equals("01:00:04:00"));
				AssertThat(t2s(doc.texts()[1]->timeOut(), PhTimeCodeType25), Equals("01:00:06:00"));

				// Detect test
				AssertThat(doc.detects().count(), Equals(0));
			});

			it("import_test03", [&]() {
				PhStripDoc doc;
				AssertThat(doc.importMosFile("test03.mos"), IsTrue());

				AssertThat(doc.videoFilePath().toStdString(), Equals("Z:\\MOT POUR MO\\AU FIL D'ARIANE_DETECTION\\jpegAFA_BOB 06_SEQ 30_PISTES SEPARES_H264.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType24));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType24), Equals("05:59:50:00"));

				// Test peoples
				AssertThat(doc.peoples().count(), Equals(2));

				PhPeople * noName = doc.peopleByName("");
				AssertThat(noName != NULL, IsTrue());
				AssertThat(noName->name().toStdString(), Equals(""));

				PhPeople * denis = doc.peopleByName("Denis");
				AssertThat(denis != NULL, IsTrue());
				AssertThat(denis->name().toStdString(), Equals("Denis"));

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
				AssertThat(doc.loops()[0]->label().toStdString(), Equals("30"));
				AssertThat(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType24), Equals("06:01:15:00"));
				AssertThat(doc.loops()[1]->label().toStdString(), Equals("off"));

				// Test texts
				AssertThat(doc.texts(false).count(), Equals(206));
				AssertThat(doc.texts(true).count(), Equals(0));

				AssertThat(doc.texts()[0]->people(), Equals(noName));
				AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType24), Equals("05:59:49:05"));
				AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType24), Equals("05:59:49:20"));

				AssertThat(doc.texts()[0]->y(), Equals(2.0f));
				AssertThat(doc.texts()[0]->height(), Equals(0.2f));

				AssertThat(doc.texts()[0]->content().toStdString(), Equals("Départ bob 6"));
				AssertThat(doc.texts()[1]->content().toStdString(), Equals("(X)"));
				AssertThat(doc.texts()[2]->content().toStdString(), Equals("05.59.50.00"));
				AssertThat(doc.texts()[3]->content().toStdString(), Equals("1000"));
				AssertThat(doc.texts()[4]->content().toStdString(), Equals("(X)"));
				AssertThat(doc.texts()[5]->content().toStdString(), Equals("P.I"));

				AssertThat(doc.texts()[9]->people(), Equals(denis));
				AssertThat(t2s(doc.texts()[9]->timeIn(), PhTimeCodeType24), Equals("06:00:07:23"));
				AssertThat(t2s(doc.texts()[9]->timeOut(), PhTimeCodeType24), Equals("06:00:08:03"));
				AssertThat(t2s(doc.texts()[10]->timeIn(), PhTimeCodeType24), Equals("06:00:08:03"));
				AssertThat(t2s(doc.texts()[10]->timeOut(), PhTimeCodeType24), Equals("06:00:08:07"));

				AssertThat(doc.texts()[9]->content().toStdString(), Equals("Tu "));
				AssertThat(doc.texts()[10]->content().toStdString(), Equals("tra"));
				AssertThat(doc.texts()[11]->content().toStdString(), Equals("vaillais "));
				AssertThat(doc.texts()[12]->content().toStdString(), Equals("pas "));
				AssertThat(doc.texts()[13]->content().toStdString(), Equals("ce "));
				AssertThat(doc.texts()[14]->content().toStdString(), Equals("soir !"));

				AssertThat(t2s(doc.texts()[14]->timeOut(), PhTimeCodeType24), Equals("06:00:09:06"));

				// Detect test
				AssertThat(doc.detects().count(), Equals(24));
				//#warning TODO more test on detect
			});

			it("import_test04", [&]() {
				PhStripDoc doc;

				AssertThat(doc.importMosFile("test04.mos"), IsTrue());

				// Test video info
				AssertThat(doc.videoFilePath().toStdString(), Equals("D:\\Ressources\\Mosaic\\Utilisateurs\\Yves\\Bold 5704\\5704.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:59:39:24"));

				// Test loops
				AssertThat(doc.loops().count(), Equals(27));
				AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:13"));
				AssertThat(doc.loops()[0]->label().toStdString(), Equals("1"));
				AssertThat(doc.loops()[1]->label().toStdString(), Equals("2"));
				AssertThat(doc.loops()[2]->label().toStdString(), Equals("3"));
				AssertThat(doc.loops()[3]->label().toStdString(), Equals("off"));
				AssertThat(doc.loops()[4]->label().toStdString(), Equals("4"));
				AssertThat(t2s(doc.loops()[5]->timeIn(), PhTimeCodeType25), Equals("01:02:56:02"));
				AssertThat(doc.loops()[5]->label().toStdString(), Equals("5"));
				AssertThat(t2s(doc.loops()[25]->timeIn(), PhTimeCodeType25), Equals("01:18:14:11"));
				AssertThat(doc.loops()[25]->label().toStdString(), Equals("25"));
				AssertThat(t2s(doc.loops()[26]->timeIn(), PhTimeCodeType25), Equals("01:19:01:01"));
				AssertThat(doc.loops()[26]->label().toStdString(), Equals("off"));

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

				AssertThat(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Gilles\\Desktop\\Sonic_EP_01_mix_VA.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType24));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType24), Equals("00:59:58:00"));

				AssertThat(doc.texts().count(), Equals(1));

				AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType24), Equals("01:00:01:00"));
				AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType24), Equals("01:00:03:00"));
			});

			it("import_mos25", [&]() {
				PhStripDoc doc;

				AssertThat(doc.importMosFile("mos25.mos"), IsTrue());

				AssertThat(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Gilles\\Desktop\\Get Blake\\Get Blake 115\\GBL_EP115_Online_Master_VA_h264_TCI.mov"));
				AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("09:59:58:00"));
			});
		});

		describe("drb", [&]() {
			describe("v5", [&]() {
				it("import_drb01", [&]() {
					AssertThat(doc.openStripFile("drb01.drb"), IsTrue());

					AssertThat(doc.videoFilePath().toStdString(), Equals("C:\\Users\\SYNCHRO USER\\Downloads\\TheManWithTheGoldenArm_25fps_high\\TheManWithTheGoldenArm_25fps_high.mov"));

					AssertThat(doc.loops().count(), Equals(0));

					AssertThat(doc.peoples().count(), Equals(2));
					AssertThat(doc.peoples()[0]->name().toStdString(), Equals("Personnage 1"));
					AssertThat(doc.peoples()[1]->name().toStdString(), Equals("Personnage 2"));

					AssertThat(doc.texts().count(), Equals(1));
					AssertThat(doc.texts()[0]->people()->name().toStdString(), Equals("Personnage 2"));

					AssertThat(doc.texts()[0]->y(), Equals(0.36f));
					AssertThat(doc.texts()[0]->height(), EqualsWithDelta(0.22666667f, 0.00001f));

#warning /// @todo should be the same than syn6
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:02:12:10"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:02:44:07"));
				});

				it("import_drb02", [&]() {
					AssertThat(doc.openStripFile("drb02.drb"), IsTrue());

					AssertThat(doc.videoFilePath().toStdString(), Equals("D:\\NED 201.mov"));
					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:04:20"));

					AssertThat(doc.loops().count(), Equals(21));

					for(int i = 0; i < 21; i++)
						AssertThat(doc.loops().at(i)->label().toStdString(), Equals(QString::number(i + 1).toStdString()));

					AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:39:02"));
					AssertThat(t2s(doc.loops()[1]->timeIn(), PhTimeCodeType25), Equals("01:02:14:23"));

					AssertThat(doc.peoples().count(), Equals(28));

					AssertThat(doc.peoples()[0]->name().toStdString(), Equals("Intervenant 1"));
					AssertThat(doc.peoples()[1]->name().toStdString(), Equals("ned"));
					AssertThat(doc.peoples()[2]->name().toStdString(), Equals("moze"));
					AssertThat(doc.peoples()[3]->name().toStdString(), Equals("suzie"));

					AssertThat(doc.texts().count(), Equals(546));
					AssertThat(doc.texts()[0]->people()->name().toStdString(), Equals("ned"));
					AssertThat(doc.texts()[0]->y(), Equals(0.213333338f));
					AssertThat(doc.texts()[0]->height(), Equals(0.28666667f));
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:13"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:01:05"));
				});

				it("import_drb03", [&]() {
					AssertThat(doc.openStripFile("drb03.drb"), IsTrue());

					AssertThat(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Matthhou\\Desktop\\The Crazy Ones\\The Crazy Ones 121\\The_Crazy_Ones_1AXB21_VOVI.mov"));
					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

					AssertThat(doc.loops().count(), Equals(1));

					AssertThat(doc.loops().at(0)->label().toStdString(), Equals("1"));

					AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

					AssertThat(doc.peoples().count(), Equals(2));

					AssertThat(doc.peoples()[0]->name().toStdString(), Equals("Pierre"));
					AssertThat(doc.peoples()[1]->name().toStdString(), Equals("Marie"));

					AssertThat(doc.texts().count(), Equals(2));

					AssertThat(doc.texts()[0]->people()->name().toStdString(), Equals("Pierre"));
					AssertThat(doc.texts()[0]->y(), Equals(0.066666667f));
					AssertThat(doc.texts()[0]->height(), Equals(0.22666667f));
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:01:00"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:03:00"));

					AssertThat(doc.texts()[1]->people()->name().toStdString(), Equals("Marie"));
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
					AssertThat(doc.peoples()[0]->name().toStdString(), Equals("Personnage 1"));
					AssertThat(doc.peoples()[1]->name().toStdString(), Equals("Personnage 2"));

					AssertThat(doc.texts().count(), Equals(1));
					AssertThat(doc.texts()[0]->people()->name().toStdString(), Equals("Personnage 1"));

					AssertThat(doc.texts()[0]->y(), Equals(0.36f));
					AssertThat(doc.texts()[0]->height(), Equals(0.22666667f));

					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("00:00:03:11"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("00:00:06:14"));
				});

				it("import_test02", [&]() {
					AssertThat(doc.openStripFile("test02.syn6"), IsTrue());

					AssertThat(doc.videoFilePath().toStdString(), Equals("C:\\Users\\Matthhou\\Desktop\\The Crazy Ones\\The Crazy Ones 121\\The_Crazy_Ones_1AXB21_VOVI.mov"));
					AssertThat(doc.videoTimeCodeType(), Equals(PhTimeCodeType25));
					AssertThat(t2s(doc.videoTimeIn(), PhTimeCodeType25), Equals("00:58:24:00"));

					AssertThat(doc.loops().count(), Equals(1));

					AssertThat(doc.loops().at(0)->label().toStdString(), Equals("1"));

					AssertThat(t2s(doc.loops()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:00:00"));

					AssertThat(doc.peoples().count(), Equals(2));

					AssertThat(doc.peoples()[0]->name().toStdString(), Equals("Pierre"));
					AssertThat(doc.peoples()[1]->name().toStdString(), Equals("Marie"));

					AssertThat(doc.texts().count(), Equals(2));

					AssertThat(doc.texts()[0]->people()->name().toStdString(), Equals("Pierre"));
					AssertThat(doc.texts()[0]->y(), Equals(0.066666667f));
					AssertThat(doc.texts()[0]->height(), Equals(0.22666667f));
					AssertThat(t2s(doc.texts()[0]->timeIn(), PhTimeCodeType25), Equals("01:00:01:00"));
					AssertThat(t2s(doc.texts()[0]->timeOut(), PhTimeCodeType25), Equals("01:00:03:00"));

					AssertThat(doc.texts()[1]->people()->name().toStdString(), Equals("Marie"));
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

			AssertThat(doc.peopleByName("Jeanne")->name().toStdString(), Equals("Jeanne"));
			AssertThat(doc.peopleByName("Sue")->name().toStdString(), Equals("Sue"));
			AssertThat(doc.peopleByName("Paul")->name().toStdString(), Equals("Paul"));
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

		it("add object", [&](){
			doc.addPeople(new PhPeople("A people"));

			doc.addObject(new PhStripText(0, doc.peoples().last(), 10000, 1, "Hello", 0.25f));
			AssertThat(doc.texts().count(), Equals(1));
			doc.addObject(new PhStripCut(5400, PhStripCut::CrossFade));
			AssertThat(doc.cuts().count(), Equals(1));
			doc.addObject(new PhStripDetect(PhStripDetect::Aperture, 10000, doc.peoples().last(), 11000, 1));
			AssertThat(doc.detects().count(), Equals(1));

			doc.addObject(new PhStripLoop(22000, "3"));
			AssertThat(doc.loops().count(), Equals(1));

		});

		it("add people", [&](){
			doc.addPeople(new PhPeople("A people"));
			AssertThat(doc.peoples().count(), Equals(1));
			doc.addPeople(new PhPeople("A second people"));
			AssertThat(doc.peoples().count(), Equals(2));
		});
	});
});
