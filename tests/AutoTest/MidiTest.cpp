/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <QThread>

#include "PhTools/PhDebug.h"
#include "PhMidi/PhMidiInput.h"
#include "PhMidi/PhMidiOutput.h"

#include "MidiTest.h"

#warning /// @todo Move to PhTest
QString MidiTest::t2s(PhTime time, PhTimeCodeType tcType)
{
	return PhTimeCode::stringFromTime(time, tcType);
}

PhTime MidiTest::s2t(QString string, PhTimeCodeType tcType)
{
	return PhTimeCode::timeFromString(string, tcType);
}

void MidiTest::initTestCase()
{
	PhDebug::enable();
}

void MidiTest::testConnection()
{
	PhMidiOutput midiOut;
	// test opening an unexisting midi port
	QVERIFY(!midiOut.open("testConnection"));

	PhMidiInput midiIn;

	// test creating a virtual port
	QVERIFY(midiIn.open("testConnection"));

	// test opening an existing midi port
	QVERIFY(midiOut.open("testConnection"));
}

void MidiTest::testTC()
{
	PhMidiInput midiIn;
	PhMidiOutput midiOut;

	int quarterFrameCount = 0;
	int tcCount = 0;
	PhTime time = 0;
	PhTimeCodeType tcType = PhTimeCodeType25;

	connect(&midiIn, &PhMidiInput::onQuarterFrame, [&]() {quarterFrameCount++; });
	connect(&midiIn, &PhMidiInput::onTC, [&](int h, int m, int s, int f, PhTimeCodeType type) {
	            tcCount++;
	            time = PhTimeCode::timeFromHhMmSsFf(h, m, s, f, type);
	            tcType = type;
			});

	QVERIFY(midiIn.open("testTC"));
	QVERIFY(midiOut.open("testTC"));

	QCOMPARE(quarterFrameCount, 0);
	QCOMPARE(tcCount, 0);
	QCOMPARE(tcType, PhTimeCodeType25);

	// Sending a quarter frame MTC message
	midiOut.sendMTC(0x01); // setting lower frame to 1
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 1);
	QCOMPARE(tcCount, 0);
	QCOMPARE(tcType, PhTimeCodeType25);

#warning /// @todo Test basic tc change
	midiOut.sendMTC(0x11); // setting higher frame to 0x1x
	QThread::msleep(10);
	midiOut.sendMTC(0x23); // setting lower second to 3
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 3);
	QCOMPARE(tcCount, 0);
	QCOMPARE(tcType, PhTimeCodeType25);

	midiOut.sendMTC(0x31); // setting higher second to 0x1x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 4);
	QCOMPARE(tcCount, 1);
	QCOMPARE(tcType, PhTimeCodeType25);
	QCOMPARE(t2s(time, tcType), QString("00:00:19:17"));

	midiOut.sendMTC(0x48); // setting lower minute to 0x08
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 5);
	QCOMPARE(tcCount, 1);

	midiOut.sendMTC(0x52); // setting higher minute to 0x2x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 6);
	QCOMPARE(tcCount, 1);

	midiOut.sendMTC(0x67); // setting lower hour to 0x07
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 7);
	QCOMPARE(tcCount, 1);

	midiOut.sendMTC(0x77); // setting rate to 30 and higher hour to 0x1x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 8);
	QCOMPARE(tcCount, 2);
	PHDEBUG << tcType;
	QCOMPARE(tcType, PhTimeCodeType30);
	QCOMPARE(t2s(time, tcType), QString("23:40:19:17"));

#warning /// @todo Test specific tc change
#warning /// @todo Test midi tc type
}
