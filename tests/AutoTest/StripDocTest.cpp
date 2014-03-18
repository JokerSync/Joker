/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhTimeCode.h"

#include "StripDocTest.h"

void StripDocTest::importDetXHeaderTest()
{
	PhStripDoc doc;
	QVERIFY(doc.importDetX("test01.detx"));

	QCOMPARE(doc.getGenerator(), QString("Cappella v0.12.5, 1"));
	QCOMPARE(doc.getTitle(), QString("Title test"));
	QCOMPARE(doc.getTranslatedTitle(), QString("Translated title"));
	QCOMPARE(doc.getEpisode(), QString("episode ref"));
	QCOMPARE(doc.getSeason(), QString("season ref"));
	QCOMPARE(doc.getVideoPath(), QString("/Path/to/video.mov"));
	QVERIFY(doc.getVideoTimestamp() == s2f("01:00:00:00"));
	QCOMPARE(QString("Pierre Dupont"), doc.getAuthorName());
	QCOMPARE(s2f("01:00:16:00"), doc.getLastFrame());

	QList<QString> metaKey = doc.getMetaKey();

	QCOMPARE(metaKey.count(), 6);
	QVERIFY(metaKey.contains("Producteur"));
	QVERIFY(metaKey.contains("Année de production"));
	QVERIFY(metaKey.contains("Distributeur"));
	QVERIFY(metaKey.contains("Réalisateur"));
	QVERIFY(metaKey.contains("Diffuseur"));
	QVERIFY(metaKey.contains("Pays d'origine"));

	QCOMPARE(doc.getMetaInformation("Producteur"), QString("Producer"));
	QCOMPARE(doc.getMetaInformation("Année de production"), QString("2014"));
	QCOMPARE(doc.getMetaInformation("Distributeur"), QString("Distributor"));
	QCOMPARE(doc.getMetaInformation("Réalisateur"), QString("Director"));
	QCOMPARE(doc.getMetaInformation("Diffuseur"), QString("Publisher"));
	QCOMPARE(doc.getMetaInformation("Pays d'origine"), QString("Country"));
	PHDEBUG << doc.getTitle();
}

void StripDocTest::importDetXNoFile()
{
	PhStripDoc doc;

	QVERIFY(!doc.importDetX("does_not_exist.detx"));
}

void StripDocTest::importDetXPeopleTest()
{
	PhStripDoc doc;
	QVERIFY(doc.importDetX("test01.detx"));

	QCOMPARE(doc.getPeoples().count(), 3);
	QVERIFY(doc.getPeoples().value("jeanne"));
	QVERIFY(doc.getPeoples().value("sue"));
	QVERIFY(doc.getPeoples().value("johnny") == NULL);
}

void StripDocTest::importDetXLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QVERIFY(doc.getLoops().count() == 2);
	QVERIFY(doc.getLoops()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:00:00", doc.getTCType()));
	QVERIFY(doc.getLoops()[1]->getTimeIn() == PhTimeCode::frameFromString("01:01:00:00", doc.getTCType()));

}

void StripDocTest::importDetXCutTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QVERIFY(doc.getCuts().count() == 1);
	QVERIFY(doc.getCuts()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:01:00", doc.getTCType()));
}

void StripDocTest::importDetXTextTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QVERIFY(doc.getTexts().count() == 5);

	QCOMPARE(doc.getTexts()[0]->getContent(), QString("Simple sentence"));
	QVERIFY(doc.getTexts()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:02:00", doc.getTCType()));
	QVERIFY(doc.getTexts()[0]->getTimeOut() == PhTimeCode::frameFromString("01:00:04:00", doc.getTCType()));
	QCOMPARE(doc.getTexts()[0]->getPeople(), doc.getPeopleByName("Jeanne"));
	QVERIFY(doc.getTexts()[0]->getTrack() == 1);

	QCOMPARE(doc.getTexts()[1]->getContent(), QString("Composed "));
	QCOMPARE(doc.getTexts()[2]->getContent(), QString("sentence"));
	QCOMPARE(doc.getTexts()[3]->getContent(), QString("Simple off sentence"));
	QCOMPARE(doc.getTexts()[4]->getContent(), QString("Composed sentence with off"));
}

void StripDocTest::importDetXDetectTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QCOMPARE(doc.getDetects().count(), 5);

	QCOMPARE(doc.getDetects()[0]->getPeople(), doc.getPeopleByName("Jeanne"));
	QVERIFY(!doc.getDetects()[0]->off());
	QVERIFY(doc.getDetects()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:02:00", doc.getTCType()));
	QVERIFY(doc.getDetects()[0]->getTimeOut() == PhTimeCode::frameFromString("01:00:04:00", doc.getTCType()));
	QCOMPARE(doc.getDetects()[0]->getTrack(), 1);

	QCOMPARE(doc.getDetects()[1]->getPeople(), doc.getPeopleByName("Sue"));
	QVERIFY(!doc.getDetects()[1]->off());
	QVERIFY(doc.getDetects()[1]->getTimeIn() == PhTimeCode::frameFromString("01:00:05:00", doc.getTCType()));
	QVERIFY(doc.getDetects()[1]->getTimeOut() == PhTimeCode::frameFromString("01:00:07:00", doc.getTCType()));
	QCOMPARE(doc.getDetects()[1]->getTrack(), 2);

	QCOMPARE(doc.getDetects()[2]->getPeople(), doc.getPeopleByName("Paul"));
	QVERIFY(doc.getDetects()[2]->off());
	QVERIFY(doc.getDetects()[2]->getTimeIn() == PhTimeCode::frameFromString("01:00:12:00", doc.getTCType()));
	QVERIFY(doc.getDetects()[2]->getTimeOut() == PhTimeCode::frameFromString("01:00:14:00", doc.getTCType()));
	QCOMPARE(doc.getDetects()[2]->getTrack(), 1);

	QCOMPARE(doc.getDetects()[3]->getPeople(), doc.getPeopleByName("Sue"));
	QVERIFY(doc.getDetects()[3]->off());
	QVERIFY(doc.getDetects()[3]->getTimeIn() == PhTimeCode::frameFromString("01:00:15:00", doc.getTCType()));
	QVERIFY(doc.getDetects()[3]->getTimeOut() == PhTimeCode::frameFromString("01:00:17:00", doc.getTCType()));
	QCOMPARE(doc.getDetects()[3]->getTrack(), 2);

	QCOMPARE(doc.getDetects()[4]->getPeople(), doc.getPeopleByName("Sue"));
	QVERIFY(doc.getDetects()[4]->off());
	QVERIFY(doc.getDetects()[4]->getTimeIn() == PhTimeCode::frameFromString("01:00:20:00", doc.getTCType()));
	QVERIFY(doc.getDetects()[4]->getTimeOut() == PhTimeCode::frameFromString("01:00:22:00", doc.getTCType()));
	QCOMPARE(doc.getDetects()[4]->getTrack(), 2);
}

void StripDocTest::importMosTest01()
{
	PhStripDoc doc;

	QVERIFY(doc.importMos("test01.mos"));

	QVERIFY(doc.getTCType() == PhTimeCodeType25);

	QCOMPARE(doc.getAuthorName(), QString("auteur"));

	QCOMPARE(doc.getVideoPath(), QString(""));
	QCOMPARE(doc.getVideoTimestamp(), 0);

	QCOMPARE(doc.getTitle(), QString("Titre VF"));
	QCOMPARE(doc.getSeason(), QString("saison"));
	QCOMPARE(doc.getEpisode(), QString("episode"));

	// Test peoples
	QCOMPARE(doc.getPeoples().count(), 1);
	PhPeople * people = doc.getPeopleByName("Nom personnage");
	QVERIFY(people != NULL);
	QCOMPARE(people->getName(), QString("Nom personnage"));

	// Test cuts
	QCOMPARE(doc.getCuts().count(), 0);

	// Test loop
	QCOMPARE(doc.getLoops().count(), 0);

	// Test texts
	QCOMPARE(doc.getTexts().count(), 3);
#warning TODO reenable this test for complete acceptance:
	QCOMPARE(doc.getTexts()[0]->getPeople(), people);
	QCOMPARE(doc.getTexts()[0]->getContent(), QString("Ceci "));
	QCOMPARE(f2s(doc.getTexts()[0]->getTimeIn()), QString("00:00:00:20"));
	QCOMPARE(f2s(doc.getTexts()[0]->getTimeOut()), QString("00:00:01:12"));

	QCOMPARE(doc.getTexts()[1]->getContent(), QString("est un"));
	QCOMPARE(f2s(doc.getTexts()[1]->getTimeIn()), QString("00:00:01:12"));
	QCOMPARE(f2s(doc.getTexts()[1]->getTimeOut()), QString("00:00:01:16"));

	QCOMPARE(doc.getTexts()[2]->getContent(), QString(" test."));
	QCOMPARE(f2s(doc.getTexts()[2]->getTimeIn()), QString("00:00:01:16"));
	QCOMPARE(f2s(doc.getTexts()[2]->getTimeOut()), QString("00:00:02:03"));

	// Detect test
	//#warning TODO more test on detect
	QCOMPARE(doc.getDetects().count(), 4);
}

void StripDocTest::importMosTest02()
{
	PhStripDoc doc;

	QVERIFY(doc.importMos("test02.mos"));

	QVERIFY(doc.getTCType() == PhTimeCodeType25);

	QCOMPARE(doc.getVideoPath(), QString("C:\\Users\\Matthhou\\Desktop\\Burn Notice\\710\\BurnNotice_BCI710_VOVI.mov"));
#warning TODO Matthias told me that the timestamp was in fact 00:58:00:00...
	QCOMPARE(f2s(doc.getVideoTimestamp()), QString("00:58:24:00"));

	// Test peoples
	QCOMPARE(doc.getPeoples().count(), 2);

	PhPeople * pierre = doc.getPeopleByName("Pierre");
	QVERIFY(pierre != NULL);
	QCOMPARE(pierre->getName(), QString("Pierre"));

	PhPeople * marie = doc.getPeopleByName("Marie");
	QVERIFY(marie != NULL);
	QCOMPARE(marie->getName(), QString("Marie"));

	// Test cuts
	QCOMPARE(doc.getCuts().count(), 1);
	QCOMPARE(f2s(doc.getCuts()[0]->getTimeIn()), QString("01:00:00:00"));

	// Test loops
	QCOMPARE(doc.getLoops().count(), 1);
	QCOMPARE(f2s(doc.getLoops()[0]->getTimeIn()), QString("01:00:00:00"));

	// Test texts
	QCOMPARE(doc.getTexts().count(), 2);
	QCOMPARE(doc.getTexts()[0]->getPeople(), pierre);
	QCOMPARE(doc.getTexts()[0]->getTrack(), 0);
	QCOMPARE(doc.getTexts()[0]->getContent(), QString("Bonjour, Marie."));
	QCOMPARE(f2s(doc.getTexts()[0]->getTimeIn()), QString("01:00:00:00"));
	QCOMPARE(f2s(doc.getTexts()[0]->getTimeOut()), QString("01:00:02:00"));

	QCOMPARE(doc.getTexts()[1]->getPeople(), marie);
	QCOMPARE(doc.getTexts()[1]->getTrack(), 3);
	QCOMPARE(doc.getTexts()[1]->getContent(), QString("Bonjour, Pierre."));
	QCOMPARE(f2s(doc.getTexts()[1]->getTimeIn()), QString("01:00:04:00"));
	QCOMPARE(f2s(doc.getTexts()[1]->getTimeOut()), QString("01:00:06:00"));

	// Detect test
	QCOMPARE(doc.getDetects().count(), 0);
}

void StripDocTest::importMosTest03()
{
	PhStripDoc doc;

	QVERIFY(doc.importMos("test03.mos"));

	QVERIFY(doc.getTCType() == PhTimeCodeType24);

	QCOMPARE(doc.getVideoPath(), QString("Z:\\MOT POUR MO\\AU FIL D'ARIANE_DETECTION\\jpegAFA_BOB 06_SEQ 30_PISTES SEPARES_H264.mov"));
	QCOMPARE(f2s(doc.getVideoTimestamp(), doc.getTCType()), QString("05:59:50:00"));

	// Test peoples
	QCOMPARE(doc.getPeoples().count(), 2);

	PhPeople * noName = doc.getPeopleByName("");
	QVERIFY(noName != NULL);
	QCOMPARE(noName->getName(), QString(""));

	PhPeople * denis = doc.getPeopleByName("Denis");
	QVERIFY(denis != NULL);
	QCOMPARE(denis->getName(), QString("Denis"));

	// Test cuts
	QCOMPARE(doc.getCuts().count(), 29);
	QCOMPARE(f2s(doc.getCuts()[0]->getTimeIn(), doc.getTCType()), QString("05:59:50:00"));
	QCOMPARE(f2s(doc.getCuts()[1]->getTimeIn(), doc.getTCType()), QString("05:59:58:00"));
	QCOMPARE(f2s(doc.getCuts()[2]->getTimeIn(), doc.getTCType()), QString("06:00:00:00"));
	QCOMPARE(f2s(doc.getCuts()[3]->getTimeIn(), doc.getTCType()), QString("06:00:05:22"));
	QCOMPARE(f2s(doc.getCuts()[4]->getTimeIn(), doc.getTCType()), QString("06:00:08:05"));

	// Test loops
	QCOMPARE(doc.getLoops().count(), 2);
#warning /// @todo test loop number and name
	QCOMPARE(f2s(doc.getLoops()[0]->getTimeIn(), doc.getTCType()), QString("06:00:01:00"));
//	QCOMPARE(doc.getLoops()[0]->getLoopNumber(), 1);
	QCOMPARE(f2s(doc.getLoops()[1]->getTimeIn(), doc.getTCType()), QString("06:01:15:00"));
//	QCOMPARE(doc.getLoops()[1]->getLoopNumber(), 2);

	// Test texts
	QCOMPARE(doc.getTexts().count(), 206);
//#warning TODO more test on text
	// Detect test
	QCOMPARE(doc.getDetects().count(), 24);
	//#warning TODO more test on detect
}

void StripDocTest::openStripFileTest()
{
	PhStripDoc doc;

	// DetX test
	QVERIFY(doc.openStripFile("test01.detx"));
	QCOMPARE(f2s(doc.getLastFrame()), QString("01:00:16:00"));
	QCOMPARE(f2s(doc.getVideoTimestamp()), QString("01:00:00:00"));

	// Mos test
	QVERIFY(doc.openStripFile("test03.mos"));
	QCOMPARE(doc.getTCType(), PhTimeCodeType24);

	// Strip file
	QVERIFY(doc.openStripFile("test.strip"));
	QCOMPARE(doc.forceRatio169(), true);
	QCOMPARE(doc.getVideoPath(), QString("test01.mov"));
	QCOMPARE(f2s(doc.getVideoTimestamp()), QString("00:59:00:00"));
	QCOMPARE(f2s(doc.getLastFrame()), QString("01:02:03:04"));

	// Test regular joker file linked to detx
	QVERIFY(doc.openStripFile("test01.joker"));
	QCOMPARE(doc.getFilePath(), QString("test01.detx"));
	QCOMPARE(doc.getVideoPath(), QString("test01.mov"));
	QCOMPARE(f2s(doc.getVideoTimestamp()), QString("01:01:00:00"));
	QCOMPARE(doc.forceRatio169(), true);
	QCOMPARE(f2s(doc.getLastFrame()), QString("01:30:00:00"));

	QCOMPARE(doc.getTitle(), QString("Title test"));
	QCOMPARE(doc.getGenerator(), QString("Cappella v0.12.5, 1"));

	// Test bad files
	QVERIFY(QFile::exists("bad_tag.joker"));
	QVERIFY(!doc.openStripFile("bad_tag.joker"));

	QVERIFY(QFile::exists("empty.joker"));
	QVERIFY(!doc.openStripFile("empty.joker"));

	// Test accepted files
	QVERIFY(QFile::exists("empty_root.joker"));
//	QVERIFY(doc.openStripFile("empty_root.joker"));

	QVERIFY(QFile::exists("empty_meta.joker"));
//	QVERIFY(doc.openStripFile("empty_meta.joker"));
}

void StripDocTest::getPeopleByNameTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QCOMPARE(doc.getPeopleByName("Jeanne")->getName(), QString("Jeanne"));
	QCOMPARE(doc.getPeopleByName("Sue")->getName(), QString("Sue"));
	QCOMPARE(doc.getPeopleByName("Paul")->getName(), QString("Paul"));
	QVERIFY(doc.getPeopleByName("Bob") == NULL);
}

void StripDocTest::getPreviousElementFrameTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QCOMPARE(doc.getPreviousElementFrame(s2f("23:00:00:00")), s2f("01:01:00:00"));
	QCOMPARE(doc.getPreviousElementFrame(s2f("01:01:00:00")), s2f("01:00:15:00"));
	QCOMPARE(doc.getPreviousElementFrame(s2f("01:00:15:00")), s2f("01:00:12:00"));
	QCOMPARE(doc.getPreviousElementFrame(s2f("01:00:00:00")), PHFRAMEMIN);
}

void StripDocTest::getNextElementFrameTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QCOMPARE(doc.getNextElementFrame(s2f("00:00:00:00")), s2f("01:00:00:00"));
	QCOMPARE(doc.getNextElementFrame(s2f("01:00:00:00")), s2f("01:00:01:00"));
	QCOMPARE(doc.getNextElementFrame(s2f("01:00:01:00")), s2f("01:00:02:00"));
	QCOMPARE(doc.getNextElementFrame(s2f("01:00:02:00")), s2f("01:00:05:00"));
	QCOMPARE(doc.getNextElementFrame(s2f("01:00:17:00")), s2f("01:01:00:00"));
	QCOMPARE(doc.getNextElementFrame(s2f("01:01:00:00")), PHFRAMEMAX);

}

void StripDocTest::getNextTextTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QVERIFY(doc.getNextText(s2f("00:00:00:00"))->getTimeIn() == s2f("01:00:02:00"));
	QVERIFY(doc.getNextText(s2f("01:00:02:00"))->getTimeIn() == s2f("01:00:05:00"));
	QVERIFY(doc.getNextText(s2f("01:00:05:00"))->getTimeIn() == s2f("01:00:06:00"));
	QVERIFY(doc.getNextText(s2f("01:00:06:00"))->getTimeIn() == s2f("01:00:12:00"));
	QVERIFY(doc.getNextText(s2f("01:00:12:00"))->getTimeIn() == s2f("01:00:15:00"));
	QVERIFY(doc.getNextText(s2f("01:00:15:00")) == NULL);
}

void StripDocTest::getNextTextTestByPeople()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	PhPeople* sue = doc.getPeopleByName("Sue");

	QVERIFY(doc.getNextText(s2f("00:00:00:00"), sue)->getTimeIn() == s2f("01:00:05:00"));
	QVERIFY(doc.getNextText(s2f("01:00:05:00"), sue)->getTimeIn() == s2f("01:00:06:00"));
	QVERIFY(doc.getNextText(s2f("01:00:06:00"), sue)->getTimeIn() == s2f("01:00:15:00"));
	QVERIFY(doc.getNextText(s2f("01:00:15:00"), sue) == NULL);
}

void StripDocTest::getNextTextTestByPeopleList()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QList<PhPeople*> peopleList;
	peopleList.append(doc.getPeopleByName("Sue"));
	peopleList.append(doc.getPeopleByName("Paul"));

	QVERIFY(doc.getNextText(s2f("00:00:00:00"), peopleList)->getTimeIn() == s2f("01:00:05:00"));
	QVERIFY(doc.getNextText(s2f("01:00:05:00"), peopleList)->getTimeIn() == s2f("01:00:06:00"));
	QVERIFY(doc.getNextText(s2f("01:00:06:00"), peopleList)->getTimeIn() == s2f("01:00:12:00"));
	QVERIFY(doc.getNextText(s2f("01:00:12:00"), peopleList)->getTimeIn() == s2f("01:00:15:00"));
	QVERIFY(doc.getNextText(s2f("01:00:15:00"), peopleList) == NULL);
}

void StripDocTest::getNextLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QVERIFY(doc.getNextLoop(s2f("00:00:00:00"))->getTimeIn() == s2f("01:00:00:00"));
	QVERIFY(doc.getNextLoop(s2f("01:00:00:00"))->getTimeIn() == s2f("01:01:00:00"));
	QVERIFY(doc.getNextLoop(s2f("01:01:00:00")) == NULL);
}

void StripDocTest::getPreviousLoopTest()
{
	PhStripDoc doc;

	QVERIFY(doc.importDetX("test01.detx"));

	QVERIFY(doc.getPreviousLoop(s2f("01:00:00:00")) == NULL);
	QVERIFY(doc.getPreviousLoop(s2f("01:01:00:00"))->getTimeIn() == s2f("01:00:00:00"));
	QVERIFY(doc.getPreviousLoop(s2f("23:00:00:00"))->getTimeIn() == s2f("01:01:00:00"));
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

	QVERIFY(doc.importDetX("notitle.detx"));
	QVERIFY(doc.getTitle() == "notitle");

}
