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

	QCOMPARE(_doc.getTitle(), QString("Title test"));
	QCOMPARE(_doc.getTranslatedTitle(), QString("Translated title"));
	QCOMPARE(_doc.getEpisode(), QString("episode ref"));
	QCOMPARE(_doc.getSeason(), QString("season ref"));
	QCOMPARE(_doc.getVideoPath(), QString("/Path/to/video.mov"));
	QVERIFY(_doc.getVideoTimestamp() == s2f("01:00:00:00"));
	QCOMPARE(QString("Pierre Dupont"), _doc.getAuthorName());
	QCOMPARE(s2f("01:00:10:00"), _doc.getLastFrame());
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

	QVERIFY(_doc.getLoops().count() == 1);
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
#warning TODO fix link off
	QVERIFY(_doc.getTexts().count() == 5);

	QCOMPARE(_doc.getTexts()[0]->getContent(), QString("Simple sentence"));
	QVERIFY(_doc.getTexts()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:02:00", _doc.getTCType()));
	QVERIFY(_doc.getTexts()[0]->getTimeOut() == PhTimeCode::frameFromString("01:00:04:00", _doc.getTCType()));
	QCOMPARE(_doc.getTexts()[0]->getPeople(), _doc.getPeoples().value("jeanne"));
	QVERIFY(_doc.getTexts()[0]->getTrack() == 1);

	QCOMPARE(_doc.getTexts()[1]->getContent(), QString("Composed "));
	QCOMPARE(_doc.getTexts()[2]->getContent(), QString("sentence"));
	QCOMPARE(_doc.getTexts()[3]->getContent(), QString("Simple off sentence"));
	QCOMPARE(_doc.getTexts()[4]->getContent(), QString("Composed sentence with off"));
}

void PhStripDocTest::importDetXOffTest()
{
	QVERIFY(_doc.importDetX("test01.detx"));

	QCOMPARE(_doc.getOffs().count(), 2);

#warning TODO more test
}

void PhStripDocTest::importMosTest01()
{
	QVERIFY(_doc.importMos("test01.mos"));

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

	// Test loop
	QCOMPARE(_doc.getLoops().count(), 0);

	// Test texts
	QCOMPARE(_doc.getTexts().count(), 3);
#warning TODO reenable this test for complete acceptance:
//	QCOMPARE(_doc.getTexts()[0]->getPeople(), people);
	QCOMPARE(_doc.getTexts()[0]->getContent(), QString("Ceci "));
	QCOMPARE(f2s(_doc.getTexts()[0]->getTimeIn()), QString("00:00:00:20"));
	QCOMPARE(f2s(_doc.getTexts()[0]->getTimeOut()), QString("00:00:01:12"));

	QCOMPARE(_doc.getTexts()[1]->getContent(), QString("est un"));
	QCOMPARE(f2s(_doc.getTexts()[1]->getTimeIn()), QString("00:00:01:12"));
	QCOMPARE(f2s(_doc.getTexts()[1]->getTimeOut()), QString("00:00:01:16"));

	QCOMPARE(_doc.getTexts()[2]->getContent(), QString(" test."));
	QCOMPARE(f2s(_doc.getTexts()[2]->getTimeIn()), QString("00:00:01:16"));
	QCOMPARE(f2s(_doc.getTexts()[2]->getTimeOut()), QString("00:00:02:03"));

}

void PhStripDocTest::importMosTest02()
{
	QVERIFY(_doc.importMos("test02.mos"));

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

	// Test loop
	QCOMPARE(_doc.getLoops().count(), 1);
	QCOMPARE(f2s(_doc.getLoops()[0]->getTimeIn()), QString("01:00:00:00"));

	// Test texts
	QCOMPARE(_doc.getTexts().count(), 2);
#warning TODO reenable this test for complete acceptance:
//	QCOMPARE(_doc.getTexts()[0]->getPeople(), pierre);
	QCOMPARE(_doc.getTexts()[0]->getContent(), QString("Bonjour, Marie."));
	QCOMPARE(f2s(_doc.getTexts()[0]->getTimeIn()), QString("01:00:00:00"));
	QCOMPARE(f2s(_doc.getTexts()[0]->getTimeOut()), QString("01:00:02:00"));

#warning TODO reenable this test for complete acceptance:
//	QCOMPARE(_doc.getTexts()[1]->getPeople(), marie);
	QCOMPARE(_doc.getTexts()[1]->getContent(), QString("Bonjour, Pierre."));
	QCOMPARE(f2s(_doc.getTexts()[1]->getTimeIn()), QString("01:00:04:00"));
	QCOMPARE(f2s(_doc.getTexts()[1]->getTimeOut()), QString("01:00:06:00"));
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

	QCOMPARE(_doc.getPreviousElementFrame(s2f("23:00:00:00")), s2f("01:00:15:00"));
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
