/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhTimeCode.h"

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
	QVERIFY(doc.videoFramestamp() == s2f("01:00:00:00"));
	QCOMPARE(QString("Pierre Dupont"), doc.authorName());
	QCOMPARE(s2f("01:00:16:00"), doc.lastFrame());

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
	QVERIFY(doc.loops()[0]->frameIn() == PhTimeCode::frameFromString("01:00:00:00", doc.timeCodeType()));
	QVERIFY(doc.loops()[1]->frameIn() == PhTimeCode::frameFromString("01:01:00:00", doc.timeCodeType()));

}

void StripDocTest::importDetXCutTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.cuts().count() == 1);
	QVERIFY(doc.cuts()[0]->frameIn() == PhTimeCode::frameFromString("01:00:01:00", doc.timeCodeType()));
}

void StripDocTest::importDetXTextTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.texts().count() == 5);

	QCOMPARE(doc.texts()[0]->content(), QString("Simple sentence"));
	QVERIFY(doc.texts()[0]->frameIn() == PhTimeCode::frameFromString("01:00:02:00", doc.timeCodeType()));
	QVERIFY(doc.texts()[0]->frameOut() == PhTimeCode::frameFromString("01:00:04:00", doc.timeCodeType()));
	QCOMPARE(doc.texts()[0]->people(), doc.peopleByName("Jeanne"));
	QVERIFY(doc.texts()[0]->track() == 1);

	QCOMPARE(doc.texts()[1]->content(), QString("Composed "));
	QCOMPARE(doc.texts()[2]->content(), QString("sentence"));
	QCOMPARE(doc.texts()[3]->content(), QString("Simple off sentence"));
	QCOMPARE(doc.texts()[4]->content(), QString("Composed sentence with off"));
}

void StripDocTest::importDetXDetectTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.detects().count(), 5);

	QCOMPARE(doc.detects()[0]->people(), doc.peopleByName("Jeanne"));
	QVERIFY(!doc.detects()[0]->off());
	QVERIFY(doc.detects()[0]->frameIn() == PhTimeCode::frameFromString("01:00:02:00", doc.timeCodeType()));
	QVERIFY(doc.detects()[0]->frameOut() == PhTimeCode::frameFromString("01:00:04:00", doc.timeCodeType()));
	QCOMPARE(doc.detects()[0]->track(), 1);

	QCOMPARE(doc.detects()[1]->people(), doc.peopleByName("Sue"));
	QVERIFY(!doc.detects()[1]->off());
	QVERIFY(doc.detects()[1]->frameIn() == PhTimeCode::frameFromString("01:00:05:00", doc.timeCodeType()));
	QVERIFY(doc.detects()[1]->frameOut() == PhTimeCode::frameFromString("01:00:07:00", doc.timeCodeType()));
	QCOMPARE(doc.detects()[1]->track(), 2);

	QCOMPARE(doc.detects()[2]->people(), doc.peopleByName("Paul"));
	QVERIFY(doc.detects()[2]->off());
	QVERIFY(doc.detects()[2]->frameIn() == PhTimeCode::frameFromString("01:00:12:00", doc.timeCodeType()));
	QVERIFY(doc.detects()[2]->frameOut() == PhTimeCode::frameFromString("01:00:14:00", doc.timeCodeType()));
	QCOMPARE(doc.detects()[2]->track(), 1);

	QCOMPARE(doc.detects()[3]->people(), doc.peopleByName("Sue"));
	QVERIFY(doc.detects()[3]->off());
	QVERIFY(doc.detects()[3]->frameIn() == PhTimeCode::frameFromString("01:00:15:00", doc.timeCodeType()));
	QVERIFY(doc.detects()[3]->frameOut() == PhTimeCode::frameFromString("01:00:17:00", doc.timeCodeType()));
	QCOMPARE(doc.detects()[3]->track(), 2);

	QCOMPARE(doc.detects()[4]->people(), doc.peopleByName("Sue"));
	QVERIFY(doc.detects()[4]->off());
	QVERIFY(doc.detects()[4]->frameIn() == PhTimeCode::frameFromString("01:00:20:00", doc.timeCodeType()));
	QVERIFY(doc.detects()[4]->frameOut() == PhTimeCode::frameFromString("01:00:22:00", doc.timeCodeType()));
	QCOMPARE(doc.detects()[4]->track(), 2);
}

void StripDocTest::importMosTest01()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test01.mos"));

	QVERIFY(doc.timeCodeType() == PhTimeCodeType25);

	QCOMPARE(doc.authorName(), QString("auteur"));

	QCOMPARE(doc.videoFilePath(), QString(""));
	QCOMPARE(f2s(doc.videoFramestamp(), doc.timeCodeType()), QString("00:00:00:00"));

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
	QCOMPARE(f2s(doc.texts()[0]->frameIn()), QString("00:00:00:20"));
	QCOMPARE(f2s(doc.texts()[0]->frameOut()), QString("00:00:01:12"));

	QCOMPARE(doc.texts()[1]->content(), QString("est un"));
	QCOMPARE(f2s(doc.texts()[1]->frameIn()), QString("00:00:01:12"));
	QCOMPARE(f2s(doc.texts()[1]->frameOut()), QString("00:00:01:16"));

	QCOMPARE(doc.texts()[2]->content(), QString(" test."));
	QCOMPARE(f2s(doc.texts()[2]->frameIn()), QString("00:00:01:16"));
	QCOMPARE(f2s(doc.texts()[2]->frameOut()), QString("00:00:02:03"));

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
	QCOMPARE(f2s(doc.videoFramestamp()), QString("00:58:24:00"));

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
	QCOMPARE(f2s(doc.cuts()[0]->frameIn()), QString("01:00:00:00"));

	// Test loops
	QCOMPARE(doc.loops().count(), 1);
	QCOMPARE(f2s(doc.loops()[0]->frameIn()), QString("01:00:00:00"));

	// Test texts
	QCOMPARE(doc.texts().count(), 2);
	QCOMPARE(doc.texts()[0]->people(), pierre);
	QCOMPARE(doc.texts()[0]->track(), 0);
	QCOMPARE(doc.texts()[0]->content(), QString("Bonjour, Marie."));
	QCOMPARE(f2s(doc.texts()[0]->frameIn()), QString("01:00:00:00"));
	QCOMPARE(f2s(doc.texts()[0]->frameOut()), QString("01:00:02:00"));

	QCOMPARE(doc.texts()[1]->people(), marie);
	QCOMPARE(doc.texts()[1]->track(), 3);
	QCOMPARE(doc.texts()[1]->content(), QString("Bonjour, Pierre."));
	QCOMPARE(f2s(doc.texts()[1]->frameIn()), QString("01:00:04:00"));
	QCOMPARE(f2s(doc.texts()[1]->frameOut()), QString("01:00:06:00"));

	// Detect test
	QCOMPARE(doc.detects().count(), 0);
}

void StripDocTest::importMosTest03()
{
	PhStripDoc doc;

	QVERIFY(doc.importMosFile("test03.mos"));

	QVERIFY(doc.timeCodeType() == PhTimeCodeType24);

	QCOMPARE(doc.videoFilePath(), QString("Z:\\MOT POUR MO\\AU FIL D'ARIANE_DETECTION\\jpegAFA_BOB 06_SEQ 30_PISTES SEPARES_H264.mov"));
	QCOMPARE(f2s(doc.videoFramestamp(), doc.timeCodeType()), QString("05:59:50:00"));

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
	QCOMPARE(f2s(doc.cuts()[0]->frameIn(), doc.timeCodeType()), QString("05:59:50:00"));
	QCOMPARE(f2s(doc.cuts()[1]->frameIn(), doc.timeCodeType()), QString("05:59:58:00"));
	QCOMPARE(f2s(doc.cuts()[2]->frameIn(), doc.timeCodeType()), QString("06:00:00:00"));
	QCOMPARE(f2s(doc.cuts()[3]->frameIn(), doc.timeCodeType()), QString("06:00:05:22"));
	QCOMPARE(f2s(doc.cuts()[4]->frameIn(), doc.timeCodeType()), QString("06:00:08:05"));

	// Test loops
	QCOMPARE(doc.loops().count(), 2);
#warning /// @todo test loop number and name
	QCOMPARE(f2s(doc.loops()[0]->frameIn(), doc.timeCodeType()), QString("06:00:01:00"));
//	QCOMPARE(doc.getLoops()[0]->getLoopNumber(), 1);
	QCOMPARE(f2s(doc.loops()[1]->frameIn(), doc.timeCodeType()), QString("06:01:15:00"));
//	QCOMPARE(doc.getLoops()[1]->getLoopNumber(), 2);

	// Test texts
	QCOMPARE(doc.texts().count(), 206);
//#warning TODO more test on text
	// Detect test
	QCOMPARE(doc.detects().count(), 24);
	//#warning TODO more test on detect
}

void StripDocTest::openStripFileTest()
{
	PhStripDoc doc;

	// DetX test
	QVERIFY(doc.openStripFile("test01.detx"));
	QCOMPARE(f2s(doc.lastFrame()), QString("01:00:16:00"));
	QCOMPARE(f2s(doc.videoFramestamp()), QString("01:00:00:00"));

	// Mos test
	QVERIFY(doc.openStripFile("test03.mos"));
	QCOMPARE(doc.timeCodeType(), PhTimeCodeType24);

	// Strip file
	QVERIFY(doc.openStripFile("test.strip"));
	QCOMPARE(doc.forceRatio169(), true);
	QCOMPARE(doc.videoFilePath(), QString("test01.mov"));
	QCOMPARE(f2s(doc.videoFramestamp()), QString("00:59:00:00"));
	QCOMPARE(f2s(doc.lastFrame()), QString("01:02:03:04"));

	// Test regular joker file linked to detx
	QVERIFY(doc.openStripFile("test01.joker"));
	QCOMPARE(doc.filePath(), QString("test01.detx"));
	QCOMPARE(doc.videoFilePath(), QString("test01.mov"));
	QCOMPARE(f2s(doc.videoFramestamp()), QString("01:01:00:00"));
	QCOMPARE(doc.forceRatio169(), true);
	QCOMPARE(f2s(doc.lastFrame()), QString("01:30:00:00"));

	QCOMPARE(doc.title(), QString("Title test"));
	QCOMPARE(doc.generator(), QString("Cappella v0.12.5, 1"));

	// Test regular joker file linked to mos
	QVERIFY(doc.openStripFile("test02.joker"));
	QCOMPARE(doc.filePath(), QString("test03.mos"));
	QCOMPARE(doc.videoFilePath(), QString("test02.mov"));
	QCOMPARE(f2s(doc.videoFramestamp(), doc.timeCodeType()), QString("01:03:00:00"));
	QCOMPARE(doc.forceRatio169(), false);
	QCOMPARE(f2s(doc.lastFrame(), doc.timeCodeType()), QString("01:20:00:00"));

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

	QVERIFY(doc1.saveStripFile("save01.joker", "01:01:01:01", !doc1.forceRatio169()));

	PhStripDoc doc2;

	QVERIFY(doc2.openStripFile("save01.joker"));

	QCOMPARE(doc2.filePath(), doc1.filePath());
	QCOMPARE(doc2.videoFilePath(), doc1.videoFilePath());
	QCOMPARE(f2s(doc2.videoFramestamp()), f2s(doc2.videoFramestamp()));
	QCOMPARE(doc2.forceRatio169(), !doc1.forceRatio169());
	QCOMPARE(f2s(doc2.lastFrame()), QString("01:01:01:01"));

	QCOMPARE(doc2.title(), doc1.title());
	QCOMPARE(doc2.generator(), doc1.generator());
}

void StripDocTest::openSaveTest02()
{
	PhStripDoc doc1;

	QVERIFY(doc1.openStripFile("test02.joker"));

	QVERIFY(doc1.saveStripFile("save02.joker", "02:02:02:02", !doc1.forceRatio169()));

	PhStripDoc doc2;

	QVERIFY(doc2.openStripFile("save02.joker"));

	QCOMPARE(doc2.filePath(), doc1.filePath());
	QCOMPARE(doc2.videoFilePath(), doc1.videoFilePath());
	QCOMPARE(f2s(doc2.videoFramestamp(), doc2.timeCodeType()), f2s(doc1.videoFramestamp(), doc1.timeCodeType()));
	QCOMPARE(doc2.forceRatio169(), !doc1.forceRatio169());
	QCOMPARE(f2s(doc2.lastFrame(), doc2.timeCodeType()), QString("02:02:02:02"));

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

void StripDocTest::getPreviousElementFrameTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.previousElementFrame(s2f("23:00:00:00")), s2f("01:01:00:00"));
	QCOMPARE(doc.previousElementFrame(s2f("01:01:00:00")), s2f("01:00:15:00"));
	QCOMPARE(doc.previousElementFrame(s2f("01:00:15:00")), s2f("01:00:12:00"));
	QCOMPARE(doc.previousElementFrame(s2f("01:00:00:00")), PHFRAMEMIN);
}

void StripDocTest::getNextElementFrameTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QCOMPARE(doc.nextElementFrame(s2f("00:00:00:00")), s2f("01:00:00:00"));
	QCOMPARE(doc.nextElementFrame(s2f("01:00:00:00")), s2f("01:00:01:00"));
	QCOMPARE(doc.nextElementFrame(s2f("01:00:01:00")), s2f("01:00:02:00"));
	QCOMPARE(doc.nextElementFrame(s2f("01:00:02:00")), s2f("01:00:05:00"));
	QCOMPARE(doc.nextElementFrame(s2f("01:00:17:00")), s2f("01:01:00:00"));
	QCOMPARE(doc.nextElementFrame(s2f("01:01:00:00")), PHFRAMEMAX);

}

void StripDocTest::getNextTextTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.nextText(s2f("00:00:00:00"))->frameIn() == s2f("01:00:02:00"));
	QVERIFY(doc.nextText(s2f("01:00:02:00"))->frameIn() == s2f("01:00:05:00"));
	QVERIFY(doc.nextText(s2f("01:00:05:00"))->frameIn() == s2f("01:00:06:00"));
	QVERIFY(doc.nextText(s2f("01:00:06:00"))->frameIn() == s2f("01:00:12:00"));
	QVERIFY(doc.nextText(s2f("01:00:12:00"))->frameIn() == s2f("01:00:15:00"));
	QVERIFY(doc.nextText(s2f("01:00:15:00")) == NULL);
}

void StripDocTest::getNextTextTestByPeople()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	PhPeople* sue = doc.peopleByName("Sue");

	QVERIFY(doc.nextText(s2f("00:00:00:00"), sue)->frameIn() == s2f("01:00:05:00"));
	QVERIFY(doc.nextText(s2f("01:00:05:00"), sue)->frameIn() == s2f("01:00:06:00"));
	QVERIFY(doc.nextText(s2f("01:00:06:00"), sue)->frameIn() == s2f("01:00:15:00"));
	QVERIFY(doc.nextText(s2f("01:00:15:00"), sue) == NULL);
}

void StripDocTest::getNextTextTestByPeopleList()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QList<PhPeople*> peopleList;
	peopleList.append(doc.peopleByName("Sue"));
	peopleList.append(doc.peopleByName("Paul"));

	QVERIFY(doc.nextText(s2f("00:00:00:00"), peopleList)->frameIn() == s2f("01:00:05:00"));
	QVERIFY(doc.nextText(s2f("01:00:05:00"), peopleList)->frameIn() == s2f("01:00:06:00"));
	QVERIFY(doc.nextText(s2f("01:00:06:00"), peopleList)->frameIn() == s2f("01:00:12:00"));
	QVERIFY(doc.nextText(s2f("01:00:12:00"), peopleList)->frameIn() == s2f("01:00:15:00"));
	QVERIFY(doc.nextText(s2f("01:00:15:00"), peopleList) == NULL);
}

void StripDocTest::getNextLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.nextLoop(s2f("00:00:00:00"))->frameIn() == s2f("01:00:00:00"));
	QVERIFY(doc.nextLoop(s2f("01:00:00:00"))->frameIn() == s2f("01:01:00:00"));
	QVERIFY(doc.nextLoop(s2f("01:01:00:00")) == NULL);
}

void StripDocTest::getPreviousLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("test01.detx"));

	QVERIFY(doc.previousLoop(s2f("01:00:00:00")) == NULL);
	QVERIFY(doc.previousLoop(s2f("01:01:00:00"))->frameIn() == s2f("01:00:00:00"));
	QVERIFY(doc.previousLoop(s2f("23:00:00:00"))->frameIn() == s2f("01:01:00:00"));
}

QString StripDocTest::f2s(PhFrame frame, PhTimeCodeType tcType)
{
	return PhTimeCode::stringFromFrame(frame, tcType);
}

PhFrame StripDocTest::s2f(QString string, PhTimeCodeType tcType)
{
	return PhTimeCode::frameFromString(string, tcType);
}

void StripDocTest::importDetXNoTitleTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetXFile("notitle.detx"));
	QVERIFY(doc.title() == "notitle");

}
