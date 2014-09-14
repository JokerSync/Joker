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
	unsigned char quarterFrameData = 255;

	connect(&midiIn, &PhMidiInput::quarterFrame, [&](unsigned char data) {
	            quarterFrameCount++;
	            quarterFrameData = data;
			});

	QVERIFY(midiIn.open("testQFTC"));
	QVERIFY(midiOut.open("testQFTC"));

	QCOMPARE(quarterFrameCount, 0);

	// Sending a quarter frame MTC message
	midiOut.sendQFTC(0x01); // setting lower frame to 1
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 1);
	QCOMPARE((int)quarterFrameData, 0x01);

	midiOut.sendQFTC(0x11); // setting higher frame to 0x1x
	QThread::msleep(20); // try to solve travis fail
	QCOMPARE(quarterFrameCount, 2);
	QCOMPARE((int)quarterFrameData, 0x11);

	midiOut.sendQFTC(0x23); // setting lower second to 3
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 3);
	QCOMPARE((int)quarterFrameData, 0x23);

	midiOut.sendQFTC(0x31); // setting higher second to 0x1x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 4);
	QCOMPARE((int)quarterFrameData, 0x31);

	midiOut.sendQFTC(0x48); // setting lower minute to 0x08
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 5);
	QCOMPARE((int)quarterFrameData, 0x48);

	midiOut.sendQFTC(0x52); // setting higher minute to 0x2x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 6);
	QCOMPARE((int)quarterFrameData, 0x52);

	midiOut.sendQFTC(0x67); // setting lower hour to 0x07
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 7);
	QCOMPARE((int)quarterFrameData, 0x67);

	midiOut.sendQFTC(0x77); // setting rate to 30 and higher hour to 0x1x
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 8);
	QCOMPARE((int)quarterFrameData, 0x77);

	midiOut.sendQFTC(0x03); // Set lower frame to 3
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 9);
	QCOMPARE((int)quarterFrameData, 0x03);
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

	connect(&midiIn, &PhMidiInput::play, [&]() {
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

	connect(&midiIn, &PhMidiInput::stop, [&]() {
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
	//
	// Initialize the midi timecode reader:
	//

	PhMidiTimeCodeReader mtcReader(PhTimeCodeType25);
	PhMidiOutput midiOut;

	PhTimeCodeType tcType = PhTimeCodeType25;
	int tcTypeCalled = 0;
	connect(&mtcReader, &PhMidiTimeCodeReader::timeCodeTypeChanged, [&](PhTimeCodeType type) {
	            tcType = type;
	            tcTypeCalled++;
			});

	QVERIFY(mtcReader.open("testMTCReader"));
	QVERIFY(midiOut.open("testMTCReader"));

	midiOut.sendFullTC(1, 0, 0, 0, PhTimeCodeType24);
	QThread::msleep(10);
	QCOMPARE(tcTypeCalled, 1);
	QCOMPARE(tcType, PhTimeCodeType24);
	QCOMPARE(mtcReader.timeCodeType(), PhTimeCodeType24);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("01:00:00:00"));

	//
	// Any quarter frame message should trigger play mode
	//

	midiOut.sendQFTC(0x02); // Send frame low digit
	QThread::msleep(10);

	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("01:00:00:00"));
	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 1));

	// Test basic playback behaviour

	midiOut.sendQFTC(0x10); // Send frame high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x20); // Send second low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x30); // Send second high digit
	QThread::msleep(10);

	// Since 4 quarter frame message have elapsed the frame increment by one
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("01:00:00:01"));

	midiOut.sendQFTC(0x40); // Send minute low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x50); // Send minute high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x61); // Send hour low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x70); // Send hour high digit and 24 fps info
	QThread::msleep(10);

	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("01:00:00:02"));

	// Send 8 quarter frame message from another timecode (23:40:19:20)

	midiOut.sendQFTC(0x06); // Send frame low digit (6 because we start the next frame transmission)
	QThread::msleep(10);
	midiOut.sendQFTC(0x11); // Send frame high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x23); // Send second low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x31); // Send second high digit
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("01:00:00:03"));
	midiOut.sendQFTC(0x48); // Send minute low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x52); // Send minute high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x67); // Send hour low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x71); // Send hour high digit and 24 fps info
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("23:40:19:22"));

	// Send the next 8 quarter frame message to check passing seconds

	midiOut.sendQFTC(0x00); // Send frame low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x10); // Send frame high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x24); // Send second low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x31); // Send second high digit
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("23:40:19:23"));
	midiOut.sendQFTC(0x48); // Send minute low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x52); // Send minute high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x67); // Send hour low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x71); // Send hour high digit and 24 fps info
	QThread::msleep(10);

	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("23:40:20:00"));

	// Test passing minutes (from 10:03:59:20 to 10:04:00:00)

	midiOut.sendQFTC(0x06); // Send frame low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x11); // Send frame high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x2b); // Send second low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x33); // Send second high digit
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("23:40:20:01"));
	midiOut.sendQFTC(0x43); // Send minute low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x50); // Send minute high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x6a); // Send hour low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x70); // Send hour high digit and 24 fps info
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("10:03:59:22"));
	midiOut.sendQFTC(0x00); // Send frame low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x10); // Send frame high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x20); // Send second low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x30); // Send second high digit
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("10:03:59:23"));
	midiOut.sendQFTC(0x44); // Send minute low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x50); // Send minute high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x6a); // Send hour low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x70); // Send hour high digit and 24 fps info
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("10:04:00:00"));
	midiOut.sendQFTC(0x02); // Send frame low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x10); // Send frame high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x20); // Send second low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x30); // Send second high digit
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("10:04:00:01"));
	midiOut.sendQFTC(0x44); // Send minute low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x50); // Send minute high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x6a); // Send hour low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x70); // Send hour high digit and 24 fps info
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("10:04:00:02"));

	// Switch to 25 fps timecode
	QCOMPARE(tcTypeCalled, 1);
	QCOMPARE(tcType, PhTimeCodeType24);

	midiOut.sendQFTC(0x04); // Send frame low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x10); // Send frame high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x20); // Send second low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x30); // Send second high digit
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("10:04:00:03"));
	midiOut.sendQFTC(0x44); // Send minute low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x50); // Send minute high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x6a); // Send hour low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x72); // Send hour high digit and 25fps info
	QThread::msleep(10);

	QCOMPARE(tcTypeCalled, 2);
	QCOMPARE(tcType, PhTimeCodeType25);
	QCOMPARE(mtcReader.timeCodeType(), PhTimeCodeType25);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType25), QString("10:04:00:04"));

	// Stop sending quarter frame MTC message should stop the reader after one frame:
#warning /// @todo QThread::msleep block the pause detector timer and QTest::qWait crashes...
//	QThread::msleep(30);
//	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 1));
//	QThread::msleep(200);
//	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 0));
}

void MidiTest::testMTCWriter()
{
	PhMidiTimeCodeWriter mtcWriter(PhTimeCodeType30);
	PhMidiInput midiIn;

	int quarterFrameCount = 0;
	unsigned char quarterFrameData = 255;

	connect(&midiIn, &PhMidiInput::quarterFrame, [&](unsigned char data) {
	            quarterFrameCount++;
	            quarterFrameData = data;
			});

	QVERIFY(midiIn.open("testMTCWriter"));
	QVERIFY(mtcWriter.open("testMTCWriter"));

	mtcWriter.clock()->setTime(s2t("23:40:19:16", PhTimeCodeType30));
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 0);

	mtcWriter.clock()->setRate(1);
	PhFrame freq = PhTimeCode::getFps(PhTimeCodeType30) * 4; // => one quarter frame frequency
	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 1);
	QCOMPARE((int)quarterFrameData, 0x02);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 2);
	QCOMPARE((int)quarterFrameData, 0x11);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 3);
	QCOMPARE((int)quarterFrameData, 0x23);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 4);
	QCOMPARE((int)quarterFrameData, 0x31);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 5);
	QCOMPARE((int)quarterFrameData, 0x48);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 6);
	QCOMPARE((int)quarterFrameData, 0x52);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 7);
	QCOMPARE((int)quarterFrameData, 0x67);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(mtcWriter.clock()->time(), s2t("23:40:19:18", PhTimeCodeType30));
	QCOMPARE(quarterFrameCount, 8);
	QCOMPARE((int)quarterFrameData, 0x70 | (0x03 << 1) | 0x01); // timecode type info + hour high digit

	// Test changing the writer timecode type:
	mtcWriter.setTimeCodeType(PhTimeCodeType25);
	freq = PhTimeCode::getFps(PhTimeCodeType25) * 4; // => one quarter frame frequency

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 9);
	QCOMPARE((int)quarterFrameData, 0x01); // With 25 fps, the next timecode will be 23:40:19:17

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 10);
	QCOMPARE((int)quarterFrameData, 0x11);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 11);
	QCOMPARE((int)quarterFrameData, 0x23);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 12);
	QCOMPARE((int)quarterFrameData, 0x31);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 13);
	QCOMPARE((int)quarterFrameData, 0x48);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 14);
	QCOMPARE((int)quarterFrameData, 0x52);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(quarterFrameCount, 15);
	QCOMPARE((int)quarterFrameData, 0x67);

	mtcWriter.clock()->tick(freq);
	QThread::msleep(10);

	QCOMPARE(mtcWriter.clock()->time(), s2t("23:40:19:17", PhTimeCodeType25));
	QCOMPARE(quarterFrameCount, 16);
	QCOMPARE((int)quarterFrameData, 0x70 | (0x01 << 1) | 0x01); // timecode type info + hour high digit

	// Test changing speed to 0
	mtcWriter.clock()->setRate(0);

	// No quarter frame message shall be sent anymore
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 16);
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 16);
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 16);
	QThread::msleep(10);
	QCOMPARE(quarterFrameCount, 16);


}

void MidiTest::testMTCReaderWithMMC()
{
	//
	// Initialize the midi timecode reader:
	//

	PhMidiTimeCodeReader mtcReader(PhTimeCodeType25);
	PhMidiOutput midiOut;

	PhTimeCodeType tcType = PhTimeCodeType25;

	QVERIFY(mtcReader.open("testMMCReader"));
	QVERIFY(midiOut.open("testMMCReader"));

	// Send goto
	midiOut.sendMMCGoto(1, 0, 0, 0, PhTimeCodeType25);
	QThread::msleep(10);
	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType24), QString("01:00:00:00"));

	// Send play
	midiOut.sendMMCPlay();
	QThread::msleep(10);
	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 1));

	// Send quarter frame message
	midiOut.sendQFTC(0x02); // Send frame low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x10); // Send frame high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x20); // Send second low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x30); // Send second high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x40); // Send minute low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x50); // Send minute high digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x61); // Send hour low digit
	QThread::msleep(10);
	midiOut.sendQFTC(0x72); // Send hour high digit and 25 fps info
	QThread::msleep(10);

	QCOMPARE(t2s(mtcReader.clock()->time(), PhTimeCodeType25), QString("01:00:00:02"));

	// Send stop
	midiOut.sendMMCStop();
	QThread::msleep(10);
	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 0));

	// Make sur quarter frame message doesn't put play back for the next 80 ms

	midiOut.sendQFTC(0x04); // Send frame low digit
	QThread::msleep(10);
	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 0));

	midiOut.sendQFTC(0x10); // Send frame high digit
	QThread::msleep(60);
	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 0));

	midiOut.sendQFTC(0x20); // Send second low digit
	QThread::msleep(20);
	QVERIFY(PhTestTools::compareFloats(mtcReader.clock()->rate(), 1));
}

