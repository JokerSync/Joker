/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <QThread>

#include "PhTools/PhDebug.h"
#include "PhTools/PhTestTools.h"
#include "PhMidi/PhMidiInput.h"
#include "PhMidi/PhMidiOutput.h"
#include "PhMidi/PhMidiTimeCodeReader.h"
#include "PhMidi/PhMidiTimeCodeWriter.h"

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

void MidiTest::testQFTC()
{
	PhMidiInput midiIn;
	PhMidiOutput midiOut;

	int quarterFrameCount = 0;
	int tcCount = 0;
	PhTime time = 0;
	PhTimeCodeType tcType = PhTimeCodeType25;

	connect(&midiIn, &PhMidiInput::quarterFrame, [&]() {quarterFrameCount++; });
	connect(&midiIn, &PhMidiInput::timeCodeReceived, [&](int h, int m, int s, int f, PhTimeCodeType type) {
	            tcCount++;
	            tcType = type;
	            time = PhTimeCode::timeFromHhMmSsFf(h, m, s, f, tcType);
			});

	QVERIFY(midiIn.open("testQFTC"));
	QVERIFY(midiOut.open("testQFTC"));

	QCOMPARE(quarterFrameCount, 0);
	QCOMPARE(tcCount, 0);
	QCOMPARE(tcType, PhTimeCodeType25);

	// Sending a quarter frame MTC message
	midiOut.sendQFTC(0x01); // setting lower frame to 1
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 1);
	QCOMPARE(tcCount, 0);

	midiOut.sendQFTC(0x11); // setting higher frame to 0x1x
	QThread::msleep(10);
	midiOut.sendQFTC(0x23); // setting lower second to 3
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 3);
	QCOMPARE(tcCount, 0);

	midiOut.sendQFTC(0x31); // setting higher second to 0x1x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 4);
	QCOMPARE(tcCount, 0);

	midiOut.sendQFTC(0x48); // setting lower minute to 0x08
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 5);
	QCOMPARE(tcCount, 0);

	midiOut.sendQFTC(0x52); // setting higher minute to 0x2x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 6);
	QCOMPARE(tcCount, 0);

	midiOut.sendQFTC(0x67); // setting lower hour to 0x07
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 7);
	QCOMPARE(tcCount, 0);

	midiOut.sendQFTC(0x77); // setting rate to 30 and higher hour to 0x1x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 8);
	QCOMPARE(tcCount, 1);
	PHDEBUG << tcType;
	QCOMPARE(tcType, PhTimeCodeType30);
	QCOMPARE(t2s(time, tcType), QString("23:40:19:17"));

#warning /// @todo Test specific tc change
#warning /// @todo Test midi tc type
}

void MidiTest::testFullTC()
{
	PhMidiInput midiIn;
	PhMidiOutput midiOut;

	int tcCount = 0;
	PhTime time = 0;
	PhTimeCodeType tcType = PhTimeCodeType25;

	connect(&midiIn, &PhMidiInput::timeCodeReceived, [&](int h, int m, int s, int f, PhTimeCodeType type) {
	            tcCount++;
	            tcType = type;
	            time = PhTimeCode::timeFromHhMmSsFf(h, m, s, f, tcType);
			});

	QVERIFY(midiIn.open("testFullTC"));
	QVERIFY(midiOut.open("testFullTC"));

	QCOMPARE(tcCount, 0);
	QCOMPARE(tcType, PhTimeCodeType25);

	midiOut.sendFullTC(1, 2, 3, 4, PhTimeCodeType2997);
	QThread::msleep(10);

	QCOMPARE(tcCount, 1);
	QCOMPARE(tcType, PhTimeCodeType2997);
	QCOMPARE(t2s(time, tcType), QString("01:02:03:04"));
}

void MidiTest::testMMCPlay()
{
	PhMidiInput midiIn;
	PhMidiOutput midiOut;

	int playCount = 0;

	connect(&midiIn, &PhMidiInput::onPlay, [&]() {
	            playCount++;
			});

	QVERIFY(midiIn.open("testMMCStop"));
	QVERIFY(midiOut.open("testMMCStop"));

	QCOMPARE(playCount, 0);

	midiOut.sendMMCPlay();
	QThread::msleep(10);

	QCOMPARE(playCount, 1);
}

void MidiTest::testMMCStop()
{
	PhMidiInput midiIn;
	PhMidiOutput midiOut;

	int stopCount = 0;

	connect(&midiIn, &PhMidiInput::onStop, [&]() {
	            stopCount++;
			});

	QVERIFY(midiIn.open("testMMCStop"));
	QVERIFY(midiOut.open("testMMCStop"));

	QCOMPARE(stopCount, 0);

	midiOut.sendMMCStop();
	QThread::msleep(10);

	QCOMPARE(stopCount, 1);
}

void MidiTest::testMMCGoto()
{
	PhMidiInput midiIn;
	PhMidiOutput midiOut;

	int tcCount = 0;
	PhTime time = 0;
	PhTimeCodeType tcType = PhTimeCodeType25;

	connect(&midiIn, &PhMidiInput::timeCodeReceived, [&](int h, int m, int s, int f, PhTimeCodeType type) {
	            tcCount++;
	            tcType = type;
	            time = PhTimeCode::timeFromHhMmSsFf(h, m, s, f, tcType);
			});

	QVERIFY(midiIn.open("testMMCGoto"));
	QVERIFY(midiOut.open("testMMCGoto"));

	QCOMPARE(tcCount, 0);
	QCOMPARE(tcType, PhTimeCodeType25);

	midiOut.sendMMCGoto(2, 3, 4, 5, PhTimeCodeType24);
	QThread::msleep(10);

	QCOMPARE(tcCount, 1);
	QCOMPARE(tcType, PhTimeCodeType24);
	QCOMPARE(t2s(time, tcType), QString("02:03:04:05"));
}

void MidiTest::testMTCReader()
{
	PhMidiTimeCodeReader mtcReader(PhTimeCodeType25);
	PhMidiOutput midiOut;

	QVERIFY(mtcReader.open("testMTCReader"));
	QVERIFY(midiOut.open("testMTCReader"));

	midiOut.sendFullTC(1, 0, 0, 0, PhTimeCodeType24);
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType25), QString("01:00:00:00"));

	// Any quarter frame message should trigger play mode
	midiOut.sendQFTC(0);
	QThread::msleep(10);

	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 1));
}

void MidiTest::testMTCWriter()
{
	PhMidiTimeCodeWriter mtcWriter(PhTimeCodeType25);
	PhMidiInput midiIn;

	int quarterFrameCount = 0;
	int tcCount = 0;
	PhTimeCodeType tcType = PhTimeCodeType24;
	PhTime time = 0;

	connect(&midiIn, &PhMidiInput::quarterFrame, [&]() {quarterFrameCount++; });
	connect(&midiIn, &PhMidiInput::timeCodeReceived, [&](int h, int m, int s, int f, PhTimeCodeType type) {
	            tcCount++;
	            tcType = type;
	            time = PhTimeCode::timeFromHhMmSsFf(h, m, s, f, tcType);
			});

	QVERIFY(midiIn.open("testMTCWriter"));
	QVERIFY(mtcWriter.open("testMTCWriter"));

	mtcWriter.clock()->setTime(s2t("01:00:00:00", PhTimeCodeType25));
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 0);
	QCOMPARE(tcCount, 1);
	QCOMPARE(tcType, PhTimeCodeType25);
	QCOMPARE(t2s(time, PhTimeCodeType25), QString("01:00:00:00"));

	mtcWriter.clock()->setRate(1);
	mtcWriter.clock()->tick(100); // 100Hz = one quarter frame
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 1);
	QCOMPARE(tcCount, 1);

	mtcWriter.clock()->tick(100);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 2);
	QCOMPARE(tcCount, 1);

	mtcWriter.clock()->tick(100);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 3);
	QCOMPARE(tcCount, 1);

	mtcWriter.clock()->tick(100);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 4);
	QCOMPARE(tcCount, 1);

	mtcWriter.clock()->tick(100);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 5);
	QCOMPARE(tcCount, 1);

	mtcWriter.clock()->tick(100);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 6);
	QCOMPARE(tcCount, 1);

	mtcWriter.clock()->tick(100);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 7);
	QCOMPARE(tcCount, 1);

	mtcWriter.clock()->tick(100);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 8);
	QCOMPARE(tcCount, 2);
}

