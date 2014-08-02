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
	PhFrame frame = 0;
	PhTimeCodeType tcType = PhTimeCodeType25;

	connect(&midiIn, &PhMidiInput::onQuarterFrame, [&]() {quarterFrameCount++;});
	connect(&midiIn, &PhMidiInput::onTC, [&](int h, int m, int s, int f, PhTimeCodeType type) {
		tcCount++;
		frame = PhTimeCode::frameFromHhMmSsFf(h, m, s, f, type);
		tcType = type;
	});

	QVERIFY(midiIn.open("testTC"));
	QVERIFY(midiOut.open("testTC"));

	QCOMPARE(quarterFrameCount, 0);
	QCOMPARE(tcCount, 0);
	QCOMPARE(tcType, PhTimeCodeType25);

	// Sending a quarter frame MTC message
	midiOut.sendMTC(0x01);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 1);
	QCOMPARE(tcCount, 0);
	QCOMPARE(tcType, PhTimeCodeType25);
}
