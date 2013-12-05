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
	QCOMPARE(_doc.getTitle(), QString("Title test"));
	QCOMPARE(_doc.getTranslatedTitle(), QString("Translated title"));
	QCOMPARE(_doc.getEpisode(), QString("episode ref"));
	QCOMPARE(_doc.getSeason(), QString("season ref"));
	QCOMPARE(_doc.getVideoPath(), QString("/Path/to/video.mov"));
	QVERIFY(_doc.getVideoTimestamp() == s2f("01:00:00:00"));
	QCOMPARE(QString("Pierre Dupont"), _doc.getAuthorName());
	QCOMPARE(s2f("01:00:10:00"), _doc.getLastFrame());
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
	QVERIFY(_doc.getLoops().count() == 1);
	QVERIFY(_doc.getLoops()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:00:00", _doc.getTCType()));
}

void PhStripDocTest::openDetXCutTest()
{
	QVERIFY(_doc.getCuts().count() == 1);
	QVERIFY(_doc.getCuts()[0]->getTimeIn() == PhTimeCode::frameFromString("01:00:01:00", _doc.getTCType()));
}

void PhStripDocTest::openDetXTextTest()
{
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

void PhStripDocTest::openDetXOffTest()
{
	QCOMPARE(_doc.getOffs().count(), 2);

#warning TODO more test
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
	QCOMPARE(_doc.getPreviousElementFrame(s2f("23:00:00:00")), s2f("01:00:15:00"));
	QCOMPARE(_doc.getPreviousElementFrame(s2f("01:00:15:00")), s2f("01:00:12:00"));
	QCOMPARE(_doc.getPreviousElementFrame(s2f("01:00:00:00")), PHFRAMEMIN);
}

void PhStripDocTest::getNextElementFrameTest()
{
	QCOMPARE(_doc.getNextElementFrame(s2f("00:00:00:00")), s2f("01:00:00:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:00:00")), s2f("01:00:01:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:01:00")), s2f("01:00:02:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:02:00")), s2f("01:00:05:00"));
	QCOMPARE(_doc.getNextElementFrame(s2f("01:00:17:00")), PHFRAMEMAX);
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

QString PhStripDocTest::f2s(PhFrame frame)
{
	return PhTimeCode::stringFromFrame(frame, _doc.getTCType());
}

PhFrame PhStripDocTest::s2f(QString string)
{
	return PhTimeCode::frameFromString(string, _doc.getTCType());
}
