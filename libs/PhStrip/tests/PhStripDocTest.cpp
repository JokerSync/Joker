/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhTimeCode.h"

#include "PhStripDocTest.h"


void PhStripDocTest::initTestCase()
{
	QString fileName = "test01.detx";
	QVERIFY(QFile(fileName).exists());
	QVERIFY(_doc.importDetX(fileName));
}

void PhStripDocTest::openDetXHeaderTest()
{
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

void PhStripDocTest::openDetXPeopleTest()
{
	QVERIFY(_doc.getPeoples().count() == 3);
	QVERIFY(_doc.getPeoples().value("jeanne"));
	QVERIFY(_doc.getPeoples().value("sue"));
	QVERIFY(_doc.getPeoples().value("johnny") == NULL);
}

void PhStripDocTest::openDetXLoopTest()
{
	QVERIFY(_doc.getLoops().count() == 2);
	QVERIFY(_doc.getLoops()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:00:00", _doc.getTCType()));
	QVERIFY(_doc.getLoops()[1]->getTimeIn() == PhTimeCode::frameFromString("01:01:00:00", _doc.getTCType()));

}

void PhStripDocTest::openDetXCutTest()
{
	QVERIFY(_doc.getCuts().count() == 1);
	QVERIFY(_doc.getCuts()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:01:00", _doc.getTCType()));
}

void PhStripDocTest::openDetXTextTest()
{
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

void PhStripDocTest::openDetXDetectTest()
{
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

void PhStripDocTest::getPeopleByNameTest()
{
	QCOMPARE(_doc.getPeopleByName("Jeanne")->getName(), QString("Jeanne"));
	QCOMPARE(_doc.getPeopleByName("Sue")->getName(), QString("Sue"));
	QCOMPARE(_doc.getPeopleByName("Paul")->getName(), QString("Paul"));
	QVERIFY(_doc.getPeopleByName("Bob") == NULL);
}

void PhStripDocTest::getPreviousElementFrameTest()
{
	QCOMPARE(_doc.getPreviousElementFrame(s2f("23:00:00:00")), s2f("01:01:00:00"));
	QCOMPARE(_doc.getPreviousElementFrame(s2f("01:01:00:00")), s2f("01:00:15:00"));
	QCOMPARE(_doc.getPreviousElementFrame(s2f("01:00:15:00")), s2f("01:00:12:00"));
	QCOMPARE(_doc.getPreviousElementFrame(s2f("01:00:00:00")), PHFRAMEMIN);
}

void PhStripDocTest::getNextElementFrameTest()
{
	QCOMPARE(_doc.getNextElementFrame(s2f("00:00:00:00")), s2f("01:00:00:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:00:00")), s2f("01:00:01:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:01:00")), s2f("01:00:02:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:02:00")), s2f("01:00:05:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:17:00")), s2f("01:01:00:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:01:00:00")), PHFRAMEMAX);

}

void PhStripDocTest::getNextTextTest()
{
	QVERIFY(_doc.getNextText(s2f("00:00:00:00"))->getTimeIn() == s2f("01:00:02:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:02:00"))->getTimeIn() == s2f("01:00:05:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:05:00"))->getTimeIn() == s2f("01:00:06:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:06:00"))->getTimeIn() == s2f("01:00:12:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:12:00"))->getTimeIn() == s2f("01:00:15:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:15:00")) == NULL);
}

void PhStripDocTest::getNextTextTestByPeople()
{
	PhPeople* sue = _doc.getPeopleByName("Sue");

	QVERIFY(_doc.getNextText(s2f("00:00:00:00"), sue)->getTimeIn() == s2f("01:00:05:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:05:00"), sue)->getTimeIn() == s2f("01:00:06:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:06:00"), sue)->getTimeIn() == s2f("01:00:15:00"));
	QVERIFY(_doc.getNextText(s2f("01:00:15:00"), sue) == NULL);
}

void PhStripDocTest::getNextTextTestByPeopleList()
{
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
