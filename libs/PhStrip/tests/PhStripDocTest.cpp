/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhTimeCode.h"

#include "PhStripDocTest.h"

void PhStripDocTest::importDetXHeaderTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QCOMPARE(_doc.getGenerator(), QString("Cappella v0.12.5, 1"));
	QCOMPARE(_doc.getTitle(), QString("Title test"));
	QCOMPARE(_doc.getTranslatedTitle(), QString("Translated title"));
	QCOMPARE(_doc.getEpisode(), QString("episode ref"));
	QCOMPARE(_doc.getSeason(), QString("season ref"));
	QCOMPARE(_doc.getVideoPath(), QString("/Path/to/video.mov"));
	QVERIFY(_doc.getVideoTimestamp() == s2f("01:00:00:00"));
	QCOMPARE(QString("Pierre Dupont"), _doc.getAuthorName());
	QCOMPARE(s2f("01:00:16:00"), _doc.getLastFrame());

	QList<QString> metaKey = _doc.getMetaKey();

	QCOMPARE(metaKey.count(), 6);
	QVERIFY(metaKey.contains("Producteur"));
	QVERIFY(metaKey.contains("Année de production"));
	QVERIFY(metaKey.contains("Distributeur"));
	QVERIFY(metaKey.contains("Réalisateur"));
	QVERIFY(metaKey.contains("Diffuseur"));
	QVERIFY(metaKey.contains("Pays d'origine"));

	QCOMPARE(_doc.getMetaInformation("Producteur"), QString("Producer"));
	QCOMPARE(_doc.getMetaInformation("Année de production"), QString("2014"));
	QCOMPARE(_doc.getMetaInformation("Distributeur"), QString("Distributor"));
	QCOMPARE(_doc.getMetaInformation("Réalisateur"), QString("Director"));
	QCOMPARE(_doc.getMetaInformation("Diffuseur"), QString("Publisher"));
	QCOMPARE(_doc.getMetaInformation("Pays d'origine"), QString("Country"));
	PHDEBUG << _doc.getTitle();
}

void PhStripDocTest::importDetXPeopleTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QVERIFY(_doc.getPeoples().count() == 3);
	QVERIFY(_doc.getPeoples().value("jeanne"));
	QVERIFY(_doc.getPeoples().value("sue"));
	QVERIFY(_doc.getPeoples().value("johnny") == NULL);
}

void PhStripDocTest::importDetXLoopTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QVERIFY(_doc.getLoops().count() == 2);
	QVERIFY(_doc.getLoops()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:00:00", _doc.getTCType()));
	QVERIFY(_doc.getLoops()[1]->getTimeIn() == PhTimeCode::frameFromString("01:01:00:00", _doc.getTCType()));

}

void PhStripDocTest::importDetXCutTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QVERIFY(_doc.getCuts().count() == 1);
	QVERIFY(_doc.getCuts()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:01:00", _doc.getTCType()));
}

void PhStripDocTest::importDetXTextTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QVERIFY(_doc.getTexts().count() == 5);

	QCOMPARE(_doc.getTexts()[0]->getContent(), QString("Simple sentence"));
	QVERIFY(_doc.getTexts()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:02:00", _doc.getTCType()));
	QVERIFY(_doc.getTexts()[0]->getTimeOut() == PhTimeCode::frameFromString("01:00:04:00", _doc.getTCType()));
	QCOMPARE(_doc.getTexts()[0]->getPeople(), _doc.getPeopleByName("Jeanne"));
	QVERIFY(_doc.getTexts()[0]->getTrack() == 1);

	QCOMPARE(_doc.getTexts()[1]->getContent(), QString("Composed "));
	QCOMPARE(_doc.getTexts()[2]->getContent(), QString("sentence"));
	QCOMPARE(_doc.getTexts()[3]->getContent(), QString("Simple off sentence"));
	QCOMPARE(_doc.getTexts()[4]->getContent(), QString("Composed sentence with off"));
}

void PhStripDocTest::importDetXDetectTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QCOMPARE(_doc.getDetects().count(), 5);

	QCOMPARE(_doc.getDetects()[0]->getPeople(), _doc.getPeopleByName("Jeanne"));
	QVERIFY(!_doc.getDetects()[0]->off());
	QVERIFY(_doc.getDetects()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:02:00", _doc.getTCType()));
	QVERIFY(_doc.getDetects()[0]->getTimeOut() == PhTimeCode::frameFromString("01:00:04:00", _doc.getTCType()));
	QCOMPARE(_doc.getDetects()[0]->getTrack(), 1);

	QCOMPARE(_doc.getDetects()[1]->getPeople(), _doc.getPeopleByName("Sue"));
	QVERIFY(!_doc.getDetects()[1]->off());
	QVERIFY(_doc.getDetects()[1]->getTimeIn() == PhTimeCode::frameFromString("01:00:05:00", _doc.getTCType()));
	QVERIFY(_doc.getDetects()[1]->getTimeOut() == PhTimeCode::frameFromString("01:00:07:00", _doc.getTCType()));
	QCOMPARE(_doc.getDetects()[1]->getTrack(), 2);

	QCOMPARE(_doc.getDetects()[2]->getPeople(), _doc.getPeopleByName("Paul"));
	QVERIFY(_doc.getDetects()[2]->off());
	QVERIFY(_doc.getDetects()[2]->getTimeIn() == PhTimeCode::frameFromString("01:00:12:00", _doc.getTCType()));
	QVERIFY(_doc.getDetects()[2]->getTimeOut() == PhTimeCode::frameFromString("01:00:14:00", _doc.getTCType()));
	QCOMPARE(_doc.getDetects()[2]->getTrack(), 1);

	QCOMPARE(_doc.getDetects()[3]->getPeople(), _doc.getPeopleByName("Sue"));
	QVERIFY(_doc.getDetects()[3]->off());
	QVERIFY(_doc.getDetects()[3]->getTimeIn() == PhTimeCode::frameFromString("01:00:15:00", _doc.getTCType()));
	QVERIFY(_doc.getDetects()[3]->getTimeOut() == PhTimeCode::frameFromString("01:00:17:00", _doc.getTCType()));
	QCOMPARE(_doc.getDetects()[3]->getTrack(), 2);

	QCOMPARE(_doc.getDetects()[4]->getPeople(), _doc.getPeopleByName("Sue"));
	QVERIFY(_doc.getDetects()[4]->off());
	QVERIFY(_doc.getDetects()[4]->getTimeIn() == PhTimeCode::frameFromString("01:00:20:00", _doc.getTCType()));
	QVERIFY(_doc.getDetects()[4]->getTimeOut() == PhTimeCode::frameFromString("01:00:22:00", _doc.getTCType()));
	QCOMPARE(_doc.getDetects()[4]->getTrack(), 2);
}

void PhStripDocTest::importMosTest01()
{
	QVERIFY(_doc.importMos("test01.mos"));

	QVERIFY(_doc.getTCType() == PhTimeCodeType25);

	QCOMPARE(_doc.getAuthorName(), QString("auteur"));

	QCOMPARE(_doc.getVideoPath(), QString(""));
	QCOMPARE(_doc.getVideoTimestamp(), 0);

	QCOMPARE(_doc.getTitle(), QString("Titre VF"));
	QCOMPARE(_doc.getSeason(), QString("saison"));
	QCOMPARE(_doc.getEpisode(), QString("episode"));

	// Test peoples
	QCOMPARE(_doc.getPeoples().count(), 1);
	PhPeople * people = _doc.getPeopleByName("Nom personnage");
	QVERIFY(people != NULL);
	QCOMPARE(people->getName(), QString("Nom personnage"));

	// Test cuts
	QCOMPARE(_doc.getCuts().count(), 0);

	// Test loop
	QCOMPARE(_doc.getLoops().count(), 0);

	// Test texts
	QCOMPARE(_doc.getTexts().count(), 3);
#warning TODO reenable this test for complete acceptance:
	QCOMPARE(_doc.getTexts()[0]->getPeople(), people);
	QCOMPARE(_doc.getTexts()[0]->getContent(), QString("Ceci "));
	QCOMPARE(f2s(_doc.getTexts()[0]->getTimeIn()), QString("00:00:00:20"));
	QCOMPARE(f2s(_doc.getTexts()[0]->getTimeOut()), QString("00:00:01:12"));

	QCOMPARE(_doc.getTexts()[1]->getContent(), QString("est un"));
	QCOMPARE(f2s(_doc.getTexts()[1]->getTimeIn()), QString("00:00:01:12"));
	QCOMPARE(f2s(_doc.getTexts()[1]->getTimeOut()), QString("00:00:01:16"));

	QCOMPARE(_doc.getTexts()[2]->getContent(), QString(" test."));
	QCOMPARE(f2s(_doc.getTexts()[2]->getTimeIn()), QString("00:00:01:16"));
	QCOMPARE(f2s(_doc.getTexts()[2]->getTimeOut()), QString("00:00:02:03"));

	// Detect test
	//#warning TODO more test on detect
	QCOMPARE(_doc.getDetects().count(), 4);
}

void PhStripDocTest::importMosTest02()
{
	QVERIFY(_doc.importMos("test02.mos"));

	QVERIFY(_doc.getTCType() == PhTimeCodeType25);

	QCOMPARE(_doc.getVideoPath(), QString("C:\\Users\\Matthhou\\Desktop\\Burn Notice\\710\\BurnNotice_BCI710_VOVI.mov"));
#warning TODO Matthias told me that the timestamp was in fact 00:58:00:00...
	QCOMPARE(f2s(_doc.getVideoTimestamp()), QString("00:58:24:00"));

	// Test peoples
	QCOMPARE(_doc.getPeoples().count(), 2);

	PhPeople * pierre = _doc.getPeopleByName("Pierre");
	QVERIFY(pierre != NULL);
	QCOMPARE(pierre->getName(), QString("Pierre"));

	PhPeople * marie = _doc.getPeopleByName("Marie");
	QVERIFY(marie != NULL);
	QCOMPARE(marie->getName(), QString("Marie"));

	// Test cuts
	QCOMPARE(_doc.getCuts().count(), 1);
	QCOMPARE(f2s(_doc.getCuts()[0]->getTimeIn()), QString("01:00:00:00"));

	// Test loops
	QCOMPARE(_doc.getLoops().count(), 1);
	QCOMPARE(f2s(_doc.getLoops()[0]->getTimeIn()), QString("01:00:00:00"));

	// Test texts
	QCOMPARE(_doc.getTexts().count(), 2);
	QCOMPARE(_doc.getTexts()[0]->getPeople(), pierre);
	QCOMPARE(_doc.getTexts()[0]->getTrack(), 0);
	QCOMPARE(_doc.getTexts()[0]->getContent(), QString("Bonjour, Marie."));
	QCOMPARE(f2s(_doc.getTexts()[0]->getTimeIn()), QString("01:00:00:00"));
	QCOMPARE(f2s(_doc.getTexts()[0]->getTimeOut()), QString("01:00:02:00"));

	QCOMPARE(_doc.getTexts()[1]->getPeople(), marie);
	QCOMPARE(_doc.getTexts()[1]->getTrack(), 3);
	QCOMPARE(_doc.getTexts()[1]->getContent(), QString("Bonjour, Pierre."));
	QCOMPARE(f2s(_doc.getTexts()[1]->getTimeIn()), QString("01:00:04:00"));
	QCOMPARE(f2s(_doc.getTexts()[1]->getTimeOut()), QString("01:00:06:00"));

	// Detect test
	QCOMPARE(_doc.getDetects().count(), 0);
}

void PhStripDocTest::importMosTest03()
{
	QVERIFY(_doc.importMos("test03.mos"));

	QVERIFY(_doc.getTCType() == PhTimeCodeType24);

	QCOMPARE(_doc.getVideoPath(), QString("Z:\\MOT POUR MO\\AU FIL D'ARIANE_DETECTION\\jpegAFA_BOB 06_SEQ 30_PISTES SEPARES_H264.mov"));
	QCOMPARE(f2s(_doc.getVideoTimestamp()), QString("05:59:50:00"));

	// Test peoples
	QCOMPARE(_doc.getPeoples().count(), 2);

	PhPeople * noName = _doc.getPeopleByName("");
	QVERIFY(noName != NULL);
	QCOMPARE(noName->getName(), QString(""));

	PhPeople * denis = _doc.getPeopleByName("Denis");
	QVERIFY(denis != NULL);
	QCOMPARE(denis->getName(), QString("Denis"));

	// Test cuts
	QCOMPARE(_doc.getCuts().count(), 29);
	QCOMPARE(f2s(_doc.getCuts()[0]->getTimeIn()), QString("05:59:50:00"));
	QCOMPARE(f2s(_doc.getCuts()[1]->getTimeIn()), QString("05:59:58:00"));
	QCOMPARE(f2s(_doc.getCuts()[2]->getTimeIn()), QString("06:00:00:00"));
	QCOMPARE(f2s(_doc.getCuts()[3]->getTimeIn()), QString("06:00:05:22"));
	QCOMPARE(f2s(_doc.getCuts()[4]->getTimeIn()), QString("06:00:08:05"));

	// Test loops
	QCOMPARE(_doc.getLoops().count(), 2);
#warning /// @todo test loop number and name
	QCOMPARE(f2s(_doc.getLoops()[0]->getTimeIn()), QString("06:00:01:00"));
//	QCOMPARE(_doc.getLoops()[0]->getLoopNumber(), 1);
	QCOMPARE(f2s(_doc.getLoops()[1]->getTimeIn()), QString("06:01:15:00"));
//	QCOMPARE(_doc.getLoops()[1]->getLoopNumber(), 2);

	// Test texts
	QCOMPARE(_doc.getTexts().count(), 206);
//#warning TODO more test on text
	// Detect test
	QCOMPARE(_doc.getDetects().count(), 24);
	//#warning TODO more test on detect
}

void PhStripDocTest::getPeopleByNameTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QCOMPARE(_doc.getPeopleByName("Jeanne")->getName(), QString("Jeanne"));
	QCOMPARE(_doc.getPeopleByName("Sue")->getName(), QString("Sue"));
	QCOMPARE(_doc.getPeopleByName("Paul")->getName(), QString("Paul"));
	QVERIFY(_doc.getPeopleByName("Bob") == NULL);
}

void PhStripDocTest::getPreviousElementFrameTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QCOMPARE(_doc.getPreviousElementFrame(s2f("23:00:00:00")), s2f("01:01:00:00"));
	QCOMPARE(_doc.getPreviousElementFrame(s2f("01:01:00:00")), s2f("01:00:15:00"));
	QCOMPARE(_doc.getPreviousElementFrame(s2f("01:00:15:00")), s2f("01:00:12:00"));
	QCOMPARE(_doc.getPreviousElementFrame(s2f("01:00:00:00")), PHFRAMEMIN);
}

void PhStripDocTest::getNextElementFrameTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QCOMPARE(_doc.getNextElementFrame(s2f("00:00:00:00")), s2f("01:00:00:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:00:00")), s2f("01:00:01:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:01:00")), s2f("01:00:02:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:02:00")), s2f("01:00:05:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:17:00")), s2f("01:01:00:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:01:00:00")), PHFRAMEMAX);

}

void PhStripDocTest::getNextTextTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QVERIFY(_doc.getNextText(s2f("00:00:00:00"))->getTimeIn() == s2f("01:00:02:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:02:00"))->getTimeIn() == s2f("01:00:05:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:05:00"))->getTimeIn() == s2f("01:00:06:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:06:00"))->getTimeIn() == s2f("01:00:12:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:12:00"))->getTimeIn() == s2f("01:00:15:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:15:00")) == NULL);
}

void PhStripDocTest::getNextTextTestByPeople()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	PhPeople* sue = _doc.getPeopleByName("Sue");

	QVERIFY(_doc.getNextText(s2f("00:00:00:00"), sue)->getTimeIn() == s2f("01:00:05:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:05:00"), sue)->getTimeIn() == s2f("01:00:06:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:06:00"), sue)->getTimeIn() == s2f("01:00:15:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:15:00"), sue) == NULL);
}

void PhStripDocTest::getNextTextTestByPeopleList()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QList<PhPeople*> peopleList;
	peopleList.append(_doc.getPeopleByName("Sue"));
	peopleList.append(_doc.getPeopleByName("Paul"));

	QVERIFY(_doc.getNextText(s2f("00:00:00:00"), peopleList)->getTimeIn() == s2f("01:00:05:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:05:00"), peopleList)->getTimeIn() == s2f("01:00:06:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:06:00"), peopleList)->getTimeIn() == s2f("01:00:12:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:12:00"), peopleList)->getTimeIn() == s2f("01:00:15:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:15:00"), peopleList) == NULL);
}

void PhStripDocTest::getNextLoopTest()
{
	QVERIFY(_doc.getNextLoop(s2f("00:00:00:00"))->getTimeIn() == s2f("01:00:00:00"));
	QVERIFY(_doc.getNextLoop(s2f("01:00:00:00"))->getTimeIn() == s2f("01:01:00:00"));
	QVERIFY(_doc.getNextLoop(s2f("01:01:00:00")) == NULL);
}

void PhStripDocTest::getPreviousLoopTest()
{
	QVERIFY(_doc.getPreviousLoop(s2f("01:00:00:00")) == NULL);
	QVERIFY(_doc.getPreviousLoop(s2f("01:01:00:00"))->getTimeIn() == s2f("01:00:00:00"));
	QVERIFY(_doc.getPreviousLoop(s2f("23:00:00:00"))->getTimeIn() == s2f("01:01:00:00"));

}

QString PhStripDocTest::f2s(PhFrame frame)
{
	return PhTimeCode::stringFromFrame(frame, _doc.getTCType());
}

PhFrame PhStripDocTest::s2f(QString string)
{
	return PhTimeCode::frameFromString(string, _doc.getTCType());
}

void PhStripDocTest::importDetXNoTitleTest()
{
	QVERIFY(_doc.importDetX("notitle.detx"));
	QVERIFY(_doc.getTitle() == "notitle.detx");

}
