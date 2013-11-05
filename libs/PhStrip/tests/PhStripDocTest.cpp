#include "PhTools/PhTimeCode.h"

#include "PhStripDocTest.h"


void PhStripDocTest::initTestCase()
{
	QString fileName = "test01.detx";
	QVERIFY(QFile(fileName).exists());
	QVERIFY(_doc.openDetX(fileName));
}

void PhStripDocTest::openDetXHeaderTest()
{
	QCOMPARE(_doc.getTitle(), QString("Title test"));
	QCOMPARE(_doc.getVideoPath(), QString("/Path/to/video.mov"));
	QVERIFY(_doc.getVideoTimestamp() == PhTimeCode::frameFromString("01:00:00:00", _doc.getTCType()));
}

void PhStripDocTest::openDetXPeopleTest()
{
	QVERIFY(_doc.getPeoples().count() == 2);
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

void PhStripDocTest::getPreviousElementFrameTest()
{
	QCOMPARE(s2f("01:00:15:00"), _doc.getPreviousElementFrame(s2f("23:00:00:00")));
	QCOMPARE(s2f("01:00:12:00"), _doc.getPreviousElementFrame(s2f("01:00:15:00")));
}

void PhStripDocTest::getNextElementFrameTest()
{
	QCOMPARE(s2f("01:00:00:00"), _doc.getNextElementFrame(s2f("00:00:00:00")));
	QCOMPARE(s2f("01:00:01:00"), _doc.getNextElementFrame(s2f("01:00:00:00")));
	QCOMPARE(s2f("01:00:02:00"), _doc.getNextElementFrame(s2f("01:00:01:00")));
	QCOMPARE(s2f("01:00:05:00"), _doc.getNextElementFrame(s2f("01:00:02:00")));
}

QString PhStripDocTest::f2s(PhFrame frame)
{
	return PhTimeCode::stringFromFrame(frame, _doc.getTCType());
}

PhFrame PhStripDocTest::s2f(QString string)
{
	return PhTimeCode::frameFromString(string, _doc.getTCType());
}
