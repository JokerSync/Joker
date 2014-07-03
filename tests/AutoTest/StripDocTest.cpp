/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhTimeCode.h"
#include "PhTools/PhTestTools.h"

#include "StripDocTest.h"

void StripDocTest::initTestCase()
{
	PhDebug::disable();
}

void StripDocTest::importDetXHeaderTest()
{
	PhStripDoc doc;
	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.generator(), QString("Cappella v0.12.5, 1"));
	QCOMPARE(doc.title(), QString("Title test"));
	QCOMPARE(doc.translatedTitle(), QString("Translated title"));
	QCOMPARE(doc.episode(), QString("episode ref"));
	QCOMPARE(doc.season(), QString("season ref"));
	QCOMPARE(doc.videoFilePath(), QString("/Path/to/video.mov"));
	QCOMPARE(t2s(doc.videoTimeIn(), doc.timeCodeType()), QString("01:00:00:00"));
	QCOMPARE(doc.authorName(), QString("Pierre Dupont"));
	QCOMPARE(t2s(doc.lastTime(), doc.timeCodeType()), QString("01:00:16:00"));

	QList<QString> metaKey = doc.metaKeys();

	QCOMPARE(metaKey.count(), 6);
	QVERIFY(metaKey.contains("Producteur"));
	QVERIFY(metaKey.contains("Année de production"));
	QVERIFY(metaKey.contains("Distributeur"));
	QVERIFY(metaKey.contains("Réalisateur"));
	QVERIFY(metaKey.contains("Diffuseur"));
	QVERIFY(metaKey.contains("Pays d'origine"));

	QCOMPARE(doc.metaInformation("Producteur"), QString("Producer"));
	QCOMPARE(doc.metaInformation("Année de production"), QString("2014"));
	QCOMPARE(doc.metaInformation("Distributeur"), QString("Distributor"));
	QCOMPARE(doc.metaInformation("Réalisateur"), QString("Director"));
	QCOMPARE(doc.metaInformation("Diffuseur"), QString("Publisher"));
	QCOMPARE(doc.metaInformation("Pays d'origine"), QString("Country"));
	PHDEBUG << doc.title();
}

void StripDocTest::importDetXNoFile()
{
	PhStripDoc doc;

	QVERIFY(!doc.importDetXFile("does_not_exist.detx"));
}

void StripDocTest::importDetXPeopleTest()
{
	PhStripDoc doc;
	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.peoples().count(), 3);
	PhPeople *jeanne = doc.peopleByName("Jeanne");
	QVERIFY(jeanne);
	QCOMPARE(jeanne->name(), QString("Jeanne"));
	QCOMPARE(jeanne->color(), QString("#00BB00"));

	PhPeople *sue = doc.peopleByName("Sue");
	QVERIFY(sue);
	QCOMPARE(sue->name(), QString("Sue"));
	QCOMPARE(sue->color(), QString("#BB0000"));

	PhPeople *paul = doc.peopleByName("Paul");
	QVERIFY(paul);
	QCOMPARE(paul->name(), QString("Paul"));
	QCOMPARE(paul->color(), QString("#0000BB"));
	QVERIFY(doc.peopleByName("Johnny") == NULL);
}

void StripDocTest::importDetXLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.loops().count() == 2);
	QCOMPARE(t2s(doc.loops()[0]->timeIn(), doc.timeCodeType()), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.loops()[1]->timeIn(), doc.timeCodeType()), QString("01:01:00:00"));

}

void StripDocTest::importDetXCutTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.cuts().count() == 1);
	QCOMPARE(t2s(doc.cuts()[0]->timeIn(), doc.timeCodeType()), QString("01:00:01:00"));
}

void StripDocTest::importDetXTextTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.texts().count() == 6);

	QCOMPARE(doc.texts()[0]->content(), QString("Simple sentence"));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), doc.timeCodeType()), QString("01:00:02:00"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), doc.timeCodeType()), QString("01:00:04:00"));
	QCOMPARE(doc.texts()[0]->people(), doc.peopleByName("Jeanne"));
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.25f), PHNQ(QString::number(doc.texts()[0]->y())));


	QCOMPARE(doc.texts()[1]->content(), QString("Composed "));
	QCOMPARE(t2s(doc.texts()[1]->timeIn(), doc.timeCodeType()), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.texts()[1]->timeOut(), doc.timeCodeType()), QString("01:00:06:00"));

	QCOMPARE(doc.texts()[2]->content(), QString("sentence"));
	QCOMPARE(t2s(doc.texts()[2]->timeIn(), doc.timeCodeType()), QString("01:00:06:00"));
	QCOMPARE(t2s(doc.texts()[2]->timeOut(), doc.timeCodeType()), QString("01:00:07:00"));

	QCOMPARE(doc.texts()[3]->content(), QString("Simple off sentence"));
	QCOMPARE(t2s(doc.texts()[3]->timeIn(), doc.timeCodeType()), QString("01:00:12:00"));
	QCOMPARE(t2s(doc.texts()[3]->timeOut(), doc.timeCodeType()), QString("01:00:14:00"));

	QCOMPARE(doc.texts()[4]->content(), QString("Composed sentence with off"));
	QCOMPARE(t2s(doc.texts()[4]->timeIn(), doc.timeCodeType()), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.texts()[4]->timeOut(), doc.timeCodeType()), QString("01:00:17:00"));

	QCOMPARE(doc.texts()[5]->content(), QString("Sentence with out not linked"));
	QCOMPARE(t2s(doc.texts()[5]->timeIn(), doc.timeCodeType()), QString("01:00:30:00"));
	QCOMPARE(t2s(doc.texts()[5]->timeOut(), doc.timeCodeType()), QString("01:00:31:04"));
}

void StripDocTest::importDetXDetectTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.detects().count(), 6);

	QCOMPARE(doc.detects()[0]->people(), doc.peopleByName("Jeanne"));
	QCOMPARE(doc.detects()[0]->type(), PhStripDetect::On);
	QCOMPARE(t2s(doc.detects()[0]->timeIn(), doc.timeCodeType()), QString("01:00:02:00"));
	QCOMPARE(t2s(doc.detects()[0]->timeOut(), doc.timeCodeType()), QString("01:00:04:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[0]->y(), 0.25f), PHNQ(QString::number(doc.detects()[0]->y())));

	QCOMPARE(doc.detects()[1]->people(), doc.peopleByName("Sue"));
	QCOMPARE(doc.detects()[1]->type(), PhStripDetect::On);
	QCOMPARE(t2s(doc.detects()[1]->timeIn(), doc.timeCodeType()), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.detects()[1]->timeOut(), doc.timeCodeType()), QString("01:00:07:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[1]->y(), 0.5f), PHNQ(QString::number(doc.detects()[1]->y())));


	QCOMPARE(doc.detects()[2]->people(), doc.peopleByName("Paul"));
	QCOMPARE(doc.detects()[2]->type(), PhStripDetect::Off);
	QCOMPARE(t2s(doc.detects()[2]->timeIn(), doc.timeCodeType()), QString("01:00:12:00"));
	QCOMPARE(t2s(doc.detects()[2]->timeOut(), doc.timeCodeType()), QString("01:00:14:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[2]->y(), 0.25f), PHNQ(QString::number(doc.detects()[2]->y())));


	QCOMPARE(doc.detects()[3]->people(), doc.peopleByName("Sue"));
	QCOMPARE(doc.detects()[3]->type(), PhStripDetect::Off);
	QCOMPARE(t2s(doc.detects()[3]->timeIn(), doc.timeCodeType()), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.detects()[3]->timeOut(), doc.timeCodeType()), QString("01:00:17:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[3]->y(), 0.5f), PHNQ(QString::number(doc.detects()[3]->y())));

	QCOMPARE(doc.detects()[4]->people(), doc.peopleByName("Sue"));
	QCOMPARE(doc.detects()[4]->type(), PhStripDetect::Off);
	QCOMPARE(t2s(doc.detects()[4]->timeIn(), doc.timeCodeType()), QString("01:00:20:00"));
	QCOMPARE(t2s(doc.detects()[4]->timeOut(), doc.timeCodeType()), QString("01:00:22:00"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[4]->y(), 0.5f), PHNQ(QString::number(doc.detects()[4]->y())));


	QCOMPARE(doc.detects()[5]->people(), doc.peopleByName("Sue"));
	QCOMPARE(doc.detects()[5]->type(), PhStripDetect::On);
	QCOMPARE(t2s(doc.detects()[5]->timeIn(), doc.timeCodeType()), QString("01:00:30:00"));
	QCOMPARE(t2s(doc.detects()[5]->timeOut(), doc.timeCodeType()), QString("01:00:31:04"));
	QVERIFY2(PhTestTools::compareFloats(doc.detects()[5]->y(), 0.5f), PHNQ(QString::number(doc.detects()[5]->y())));

}

void StripDocTest::importMosTest01()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test01.mos"));

	QVERIFY(doc.timeCodeType() == PhTimeCodeType25);

	QCOMPARE(doc.authorName(), QString("auteur"));

	QCOMPARE(doc.videoFilePath(), QString(""));
	QCOMPARE(t2s(doc.videoTimeIn(), doc.timeCodeType()), QString("00:00:00:00"));

	QCOMPARE(doc.title(), QString("Titre VO"));
	QCOMPARE(doc.season(), QString("saison"));
	QCOMPARE(doc.episode(), QString("episode"));

	// Test peoples
	QCOMPARE(doc.peoples().count(), 1);
	PhPeople * people = doc.peopleByName("Nom personnage");
	QVERIFY(people != NULL);
	QCOMPARE(people->name(), QString("Nom personnage"));

	// Test cuts
	QCOMPARE(doc.cuts().count(), 0);

	// Test loop
	QCOMPARE(doc.loops().count(), 0);

	// Test texts
	QCOMPARE(doc.texts().count(), 3);
	QCOMPARE(doc.texts()[0]->people(), people);
	QCOMPARE(doc.texts()[0]->content(), QString("Ceci "));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), doc.timeCodeType()), QString("00:00:00:20"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), doc.timeCodeType()), QString("00:00:01:12"));

	QCOMPARE(doc.texts()[1]->content(), QString("est un"));
	QCOMPARE(t2s(doc.texts()[1]->timeIn(), doc.timeCodeType()), QString("00:00:01:12"));
	QCOMPARE(t2s(doc.texts()[1]->timeOut(), doc.timeCodeType()), QString("00:00:01:16"));

	QCOMPARE(doc.texts()[2]->content(), QString(" test."));
	QCOMPARE(t2s(doc.texts()[2]->timeIn(), doc.timeCodeType()), QString("00:00:01:16"));
	QCOMPARE(t2s(doc.texts()[2]->timeOut(), doc.timeCodeType()), QString("00:00:02:03"));

	// Detect test
	//#warning TODO more test on detect
	QCOMPARE(doc.detects().count(), 4);
}

void StripDocTest::importMosTest02()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test02.mos"));

	QVERIFY(doc.timeCodeType() == PhTimeCodeType25);

	QCOMPARE(doc.videoFilePath(), QString("C:\\Users\\Matthhou\\Desktop\\Burn Notice\\710\\BurnNotice_BCI710_VOVI.mov"));
#warning TODO Matthias told me that the timestamp was in fact 00:58:00:00...
	QCOMPARE(t2s(doc.videoTimeIn(), doc.timeCodeType()), QString("00:58:24:00"));

	// Test peoples
	QCOMPARE(doc.peoples().count(), 2);

	PhPeople * pierre = doc.peopleByName("Pierre");
	QVERIFY(pierre != NULL);
	QCOMPARE(pierre->name(), QString("Pierre"));

	PhPeople * marie = doc.peopleByName("Marie");
	QVERIFY(marie != NULL);
	QCOMPARE(marie->name(), QString("Marie"));

	// Test cuts
	QCOMPARE(doc.cuts().count(), 1);
	QCOMPARE(t2s(doc.cuts()[0]->timeIn(), doc.timeCodeType()), QString("01:00:00:00"));

	// Test loops
	QCOMPARE(doc.loops().count(), 1);
	QCOMPARE(t2s(doc.loops()[0]->timeIn(), doc.timeCodeType()), QString("01:00:00:00"));

	// Test texts
	QCOMPARE(doc.texts().count(), 2);
	QCOMPARE(doc.texts()[0]->people(), pierre);
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[0]->y(), 0.0), PHNQ(QString::number(doc.texts()[0]->y())));
	QCOMPARE(doc.texts()[0]->content(), QString("Bonjour, Marie."));
	QCOMPARE(t2s(doc.texts()[0]->timeIn(), doc.timeCodeType()), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.texts()[0]->timeOut(), doc.timeCodeType()), QString("01:00:02:00"));

	QCOMPARE(doc.texts()[1]->people(), marie);
	QVERIFY2(PhTestTools::compareFloats(doc.texts()[1]->y(), 0.6), PHNQ(QString::number(doc.texts()[1]->y())));
	QCOMPARE(doc.texts()[1]->content(), QString("Bonjour, Pierre."));
	QCOMPARE(t2s(doc.texts()[1]->timeIn(), doc.timeCodeType()), QString("01:00:04:00"));
	QCOMPARE(t2s(doc.texts()[1]->timeOut(), doc.timeCodeType()), QString("01:00:06:00"));

	// Detect test
	QCOMPARE(doc.detects().count(), 0);
}

void StripDocTest::importMosTest03()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test03.mos"));

	QVERIFY(doc.timeCodeType() == PhTimeCodeType24);

	QCOMPARE(doc.videoFilePath(), QString("Z:\\MOT POUR MO\\AU FIL D'ARIANE_DETECTION\\jpegAFA_BOB 06_SEQ 30_PISTES SEPARES_H264.mov"));
	QCOMPARE(t2s(doc.videoTimeIn(), doc.timeCodeType()), QString("05:59:50:00"));

	// Test peoples
	QCOMPARE(doc.peoples().count(), 2);

	PhPeople * noName = doc.peopleByName("");
	QVERIFY(noName != NULL);
	QCOMPARE(noName->name(), QString(""));

	PhPeople * denis = doc.peopleByName("Denis");
	QVERIFY(denis != NULL);
	QCOMPARE(denis->name(), QString("Denis"));

	// Test cuts
	QCOMPARE(doc.cuts().count(), 29);
	QCOMPARE(t2s(doc.cuts()[0]->timeIn(), doc.timeCodeType()), QString("05:59:50:00"));
	QCOMPARE(t2s(doc.cuts()[1]->timeIn(), doc.timeCodeType()), QString("05:59:58:00"));
	QCOMPARE(t2s(doc.cuts()[2]->timeIn(), doc.timeCodeType()), QString("06:00:00:00"));
	QCOMPARE(t2s(doc.cuts()[3]->timeIn(), doc.timeCodeType()), QString("06:00:05:22"));
	QCOMPARE(t2s(doc.cuts()[4]->timeIn(), doc.timeCodeType()), QString("06:00:08:05"));

	// Test loops
	QCOMPARE(doc.loops().count(), 2);
#warning /// @todo test loop number and name
	QCOMPARE(t2s(doc.loops()[0]->timeIn(), doc.timeCodeType()), QString("06:00:01:00"));
//	QCOMPARE(doc.getLoops()[0]->getLoopNumber(), 1);
	QCOMPARE(t2s(doc.loops()[1]->timeIn(), doc.timeCodeType()), QString("06:01:15:00"));
//	QCOMPARE(doc.getLoops()[1]->getLoopNumber(), 2);

	// Test texts
	QCOMPARE(doc.texts(false).count(), 206);
	QCOMPARE(doc.texts(true).count(), 0);
//#warning TODO more test on text
	// Detect test
	QCOMPARE(doc.detects().count(), 24);
	//#warning TODO more test on detect
}

void StripDocTest::importMosTest04()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test04.mos"));

	QCOMPARE(doc.texts().count(), 1118);
	QCOMPARE(doc.texts(true).count(), 4329);
	QCOMPARE(doc.detects().count(), 4552);

	QCOMPARE(t2s(doc.detects()[0]->timeIn(), doc.timeCodeType()), QString("01:00:01:13"));
	QCOMPARE(t2s(doc.detects()[0]->timeOut(), doc.timeCodeType()), QString("01:00:02:20"));
	QCOMPARE(doc.detects()[0]->type(), PhStripDetect::Off);
}

void StripDocTest::openStripFileTest()
{
	PhStripDoc doc;

	// DetX test
	QVERIFY(doc.openStripFile("test01.detx"));
	QCOMPARE(t2s(doc.lastTime(), doc.timeCodeType()), QString("01:00:16:00"));
	QCOMPARE(t2s(doc.videoTimeIn(), doc.timeCodeType()), QString("01:00:00:00"));

	// Mos test
	QVERIFY(doc.openStripFile("test03.mos"));
	QCOMPARE(doc.timeCodeType(), PhTimeCodeType24);

	// Strip file
	QVERIFY(doc.openStripFile("test.strip"));
	QCOMPARE(doc.forceRatio169(), true);
	QCOMPARE(doc.videoFilePath(), QString("test01.mov"));
	QCOMPARE(t2s(doc.videoTimeIn(), doc.timeCodeType()), QString("00:59:00:00"));
	QCOMPARE(t2s(doc.lastTime(), doc.timeCodeType()), QString("01:02:03:04"));

	// Test regular joker file linked to detx
	QVERIFY(doc.openStripFile("test01.joker"));
	QCOMPARE(doc.filePath(), QString("test01.detx"));
	QCOMPARE(doc.videoFilePath(), QString("test01.mov"));
	QCOMPARE(t2s(doc.videoTimeIn(), doc.timeCodeType()), QString("01:01:00:00"));
	QCOMPARE(doc.forceRatio169(), true);
	QCOMPARE(t2s(doc.lastTime(), doc.timeCodeType()), QString("01:30:00:00"));

	QCOMPARE(doc.title(), QString("Title test"));
	QCOMPARE(doc.generator(), QString("Cappella v0.12.5, 1"));

	// Test regular joker file linked to mos
	QVERIFY(doc.openStripFile("test02.joker"));
	QCOMPARE(doc.filePath(), QString("test03.mos"));
	QCOMPARE(doc.videoFilePath(), QString("test02.mov"));
	QCOMPARE(t2s(doc.videoTimeIn(), doc.timeCodeType()), QString("01:03:00:00"));
	QCOMPARE(doc.forceRatio169(), false);
	QCOMPARE(t2s(doc.lastTime(), doc.timeCodeType()), QString("01:20:00:00"));

	QCOMPARE(doc.title(), QString("Au fil d'Ariane"));
	QCOMPARE(doc.generator(), QString("Mosaic"));

	// Test bad files
	QVERIFY(QFile::exists("bad_tag.joker"));
	QVERIFY(!doc.openStripFile("bad_tag.joker"));

	QVERIFY(QFile::exists("empty.joker"));
	QVERIFY(!doc.openStripFile("empty.joker"));

	// Test accepted files
	QVERIFY(QFile::exists("empty_root.joker"));
	QVERIFY(doc.openStripFile("empty_root.joker"));

	QVERIFY(QFile::exists("empty_meta.joker"));
	QVERIFY(doc.openStripFile("empty_meta.joker"));
}

void StripDocTest::openSaveTest01()
{
	PhStripDoc doc1;

	QVERIFY(doc1.openStripFile("test01.joker"));

	QVERIFY(doc1.saveStripFile("save01.joker", "01:01:01:01"));

	PhStripDoc doc2;

	QVERIFY(doc2.openStripFile("save01.joker"));

	QCOMPARE(doc2.filePath(), doc1.filePath());
	QCOMPARE(doc2.videoFilePath(), doc1.videoFilePath());
	QCOMPARE(t2s(doc2.videoTimeIn(), doc2.timeCodeType()), t2s(doc1.videoTimeIn(), doc1.timeCodeType()));
	QCOMPARE(doc2.forceRatio169(), true);
	QCOMPARE(doc2.videoDeinterlace(), false);
	QCOMPARE(t2s(doc2.lastTime(), doc2.timeCodeType()), QString("01:01:01:01"));

	QCOMPARE(doc2.title(), doc1.title());
	QCOMPARE(doc2.generator(), doc1.generator());
}

void StripDocTest::openSaveTest02()
{
	PhStripDoc doc1;

	QVERIFY(doc1.openStripFile("test02.joker"));

	QVERIFY(doc1.saveStripFile("save02.joker", "02:02:02:02"));

	PhStripDoc doc2;

	QVERIFY(doc2.openStripFile("save02.joker"));

	QCOMPARE(doc2.filePath(), doc1.filePath());
	QCOMPARE(doc2.videoFilePath(), doc1.videoFilePath());
	QCOMPARE(t2s(doc2.videoTimeIn(), doc2.timeCodeType()), t2s(doc1.videoTimeIn(), doc1.timeCodeType()));
	QCOMPARE(doc2.forceRatio169(), false);
	QCOMPARE(doc2.videoDeinterlace(), true);
	QCOMPARE(t2s(doc2.lastTime(), doc2.timeCodeType()), QString("02:02:02:02"));

	QCOMPARE(doc2.title(), doc1.title());
	QCOMPARE(doc2.generator(), doc1.generator());
}

void StripDocTest::getPeopleByNameTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.peopleByName("Jeanne")->name(), QString("Jeanne"));
	QCOMPARE(doc.peopleByName("Sue")->name(), QString("Sue"));
	QCOMPARE(doc.peopleByName("Paul")->name(), QString("Paul"));
	QVERIFY(doc.peopleByName("Bob") == NULL);
}

void StripDocTest::getPreviousElementTimeTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = doc.timeCodeType();

	QCOMPARE(t2s(doc.previousElementTime(s2t("23:00:00:00", tcType)), tcType), QString("01:01:00:00"));
	QCOMPARE(t2s(doc.previousElementTime(s2t("01:01:00:00", tcType)), tcType), QString("01:00:30:00"));
	QCOMPARE(t2s(doc.previousElementTime(s2t("01:00:30:00", tcType)), tcType), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.previousElementTime(s2t("01:00:15:00", tcType)), tcType), QString("01:00:12:00"));
	QCOMPARE(doc.previousElementTime(s2t("01:00:00:00", tcType)), PHTIMEMIN);
}

void StripDocTest::getNextElementTimeTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = doc.timeCodeType();

	QCOMPARE(t2s(doc.nextElementTime(s2t("00:00:00:00", tcType)), tcType), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:00:00", tcType)), tcType), QString("01:00:01:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:01:00", tcType)), tcType), QString("01:00:02:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:02:00", tcType)), tcType), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:17:00", tcType)), tcType), QString("01:00:30:00"));
	QCOMPARE(t2s(doc.nextElementTime(s2t("01:00:30:00", tcType)), tcType), QString("01:01:00:00"));
	QCOMPARE(doc.nextElementTime(s2t("01:01:00:00", tcType)), PHTIMEMAX);

}

void StripDocTest::getNextTextTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = doc.timeCodeType();

	QCOMPARE(t2s(doc.nextText(s2t("00:00:00:00", tcType))->timeIn(), tcType), QString("01:00:02:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:02:00", tcType))->timeIn(), tcType), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:05:00", tcType))->timeIn(), tcType), QString("01:00:06:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:06:00", tcType))->timeIn(), tcType), QString("01:00:12:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:12:00", tcType))->timeIn(), tcType), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.nextText(s2t("01:00:15:00", tcType))->timeIn(), tcType), QString("01:00:30:00"));
	QVERIFY(doc.nextText(s2t("01:00:30:00", tcType)) == NULL);
}

void StripDocTest::getNextTextTestByPeople()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = doc.timeCodeType();

	PhPeople* sue = doc.peopleByName("Sue");

	QCOMPARE(t2s(doc.nextText(sue, s2t("00:00:00:00", tcType))->timeIn(), tcType), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.nextText(sue, s2t("01:00:05:00", tcType))->timeIn(), tcType), QString("01:00:06:00"));
	QCOMPARE(t2s(doc.nextText(sue, s2t("01:00:06:00", tcType))->timeIn(), tcType), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.nextText(sue, s2t("01:00:15:00", tcType))->timeIn(), tcType), QString("01:00:30:00"));
	QVERIFY(doc.nextText(sue, s2t("01:00:30:00", tcType)) == NULL);
}

void StripDocTest::getNextTextTestByPeopleList()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = doc.timeCodeType();

	QList<PhPeople*> peopleList;
	peopleList.append(doc.peopleByName("Sue"));
	peopleList.append(doc.peopleByName("Paul"));

	QCOMPARE(t2s(doc.nextText(peopleList, s2t("00:00:00:00", tcType))->timeIn(), tcType), QString("01:00:05:00"));
	QCOMPARE(t2s(doc.nextText(peopleList, s2t("01:00:05:00", tcType))->timeIn(), tcType), QString("01:00:06:00"));
	QCOMPARE(t2s(doc.nextText(peopleList, s2t("01:00:06:00", tcType))->timeIn(), tcType), QString("01:00:12:00"));
	QCOMPARE(t2s(doc.nextText(peopleList, s2t("01:00:12:00", tcType))->timeIn(), tcType), QString("01:00:15:00"));
	QCOMPARE(t2s(doc.nextText(peopleList, s2t("01:00:15:00", tcType))->timeIn(), tcType), QString("01:00:30:00"));
	QVERIFY(doc.nextText(peopleList, s2t("01:00:30:00", tcType)) == NULL);
}

void StripDocTest::getNextLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = doc.timeCodeType();

	QCOMPARE(t2s(doc.nextLoop(s2t("00:00:00:00", tcType))->timeIn(), tcType), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.nextLoop(s2t("01:00:00:00", tcType))->timeIn(), tcType), QString("01:01:00:00"));
	QVERIFY(doc.nextLoop(s2t("01:01:00:00", tcType)) == NULL);
}

void StripDocTest::getPreviousLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));
	PhTimeCodeType tcType = doc.timeCodeType();

	QVERIFY(doc.previousLoop(s2t("01:00:00:00", tcType)) == NULL);
	QCOMPARE(t2s(doc.previousLoop(s2t("01:01:00:00", tcType))->timeIn(), tcType), QString("01:00:00:00"));
	QCOMPARE(t2s(doc.previousLoop(s2t("23:00:00:00", tcType))->timeIn(), tcType), QString("01:01:00:00"));
}

void StripDocTest::addObjectTest()
{
	PhStripDoc doc;
	doc.addPeople(new PhPeople("A people"));

	doc.addObject(new PhStripText(0, doc.peoples().last(), 10000, 1, "Hello", 0.25f));
	QVERIFY(doc.texts().count() == 1);
	doc.addObject(new PhStripCut(PhStripCut::CrossFade, 5400));
	QVERIFY(doc.cuts().count() == 1);
	doc.addObject(new PhStripDetect(PhStripDetect::Aperture, 10000, doc.peoples().last(), 11000, 1));
	QVERIFY(doc.detects().count() == 1);

	doc.addObject(new PhStripLoop(3, 22000));
	QVERIFY(doc.loops().count() == 1);

}

void StripDocTest::addPeopleTest()
{
	PhStripDoc doc;
	doc.addPeople(new PhPeople("A people"));
	QVERIFY(doc.peoples().count() == 1);
	doc.addPeople(new PhPeople("A second people"));
	QVERIFY(doc.peoples().count() == 2);

}

QString StripDocTest::t2s(PhTime time, PhTimeCodeType tcType)
{
	return PhTimeCode::stringFromTime(time, tcType);
}

PhTime StripDocTest::s2t(QString string, PhTimeCodeType tcType)
{
	return PhTimeCode::timeFromString(string, tcType);
}

void StripDocTest::importDetXNoTitleTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("notitle.detx"));
	QVERIFY(doc.title() == "notitle");

}
