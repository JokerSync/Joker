/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QThread>

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"
#include "PhMidi/PhMidiInput.h"
#include "PhMidi/PhMidiOutput.h"
#include "PhMidi/PhMidiTimeCodeReader.h"
#include "PhMidi/PhMidiTimeCodeWriter.h"

#include "CommonSpec.h"

using namespace igloo;

Describe(midi_test) {
	It(connect_to_a_port) {
		PhMidiOutput midiOut;
		// test opening an unexisting midi port
		Assert::That(!midiOut.open("testConnection"));

		PhMidiInput midiIn;

		// test creating a virtual port
		Assert::That(midiIn.open("testConnection"));

		// test opening an existing midi port
		Assert::That(midiOut.open("testConnection"));
	}

	It(send_and_receive_quarter_frame_midi_timecode_message) {
		PhMidiInput midiIn;
		PhMidiOutput midiOut;

		int quarterFrameCount = 0;
		unsigned char quarterFrameData = 255;

		QObject::connect(&midiIn, &PhMidiInput::quarterFrame, [&](unsigned char data) {
			quarterFrameCount++;
			quarterFrameData = data;
		});

		Assert::That(midiIn.open("testQFTC"));
		Assert::That(midiOut.open("testQFTC"));

		Assert::That(quarterFrameCount, Equals(0));

		// Sending a quarter frame MTC message
		midiOut.sendQFTC(0x01); // setting lower frame to 1
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(1));
		Assert::That(quarterFrameData, Equals(0x01));

		midiOut.sendQFTC(0x11); // setting higher frame to 0x1x
		QThread::msleep(20); // try to solve travis fail
		Assert::That(quarterFrameCount, Equals(2));
		Assert::That(quarterFrameData, Equals(0x11));

		midiOut.sendQFTC(0x23); // setting lower second to 3
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(3));
		Assert::That(quarterFrameData, Equals(0x23));

		midiOut.sendQFTC(0x31); // setting higher second to 0x1x
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(4));
		Assert::That(quarterFrameData, Equals(0x31));

		midiOut.sendQFTC(0x48); // setting lower minute to 0x08
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(5));
		Assert::That(quarterFrameData, Equals(0x48));

		midiOut.sendQFTC(0x52); // setting higher minute to 0x2x
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(6));
		Assert::That(quarterFrameData, Equals(0x52));

		midiOut.sendQFTC(0x67); // setting lower hour to 0x07
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(7));
		Assert::That(quarterFrameData, Equals(0x67));

		midiOut.sendQFTC(0x77); // setting rate to 30 and higher hour to 0x1x
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(8));
		Assert::That(quarterFrameData, Equals(0x77));

		midiOut.sendQFTC(0x03); // Set lower frame to 3
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(9));
		Assert::That(quarterFrameData, Equals(0x03));
	}

	It(send_and_receive_full_midi_timecode_message) {
		PhMidiInput midiIn;
		PhMidiOutput midiOut;

		int tcCount = 0;
		PhTime time = 0;
		PhTimeCodeType tcType = PhTimeCodeType25;

		QObject::connect(&midiIn, &PhMidiInput::timeCodeReceived, [&](int h, int m, int s, int f, PhTimeCodeType type) {
			tcCount++;
			tcType = type;
			time = PhTimeCode::timeFromHhMmSsFf(h, m, s, f, tcType);
		});

		Assert::That(midiIn.open("testFullTC"));
		Assert::That(midiOut.open("testFullTC"));

		Assert::That(tcCount, Equals(0));
		Assert::That(tcType, Equals(PhTimeCodeType25));

		midiOut.sendFullTC(1, 2, 3, 4, PhTimeCodeType2997);
		QThread::msleep(10);

		Assert::That(tcCount, Equals(1));
		Assert::That(tcType, Equals(PhTimeCodeType2997));
		Assert::That(t2s(time, tcType), Equals("01:02:03:04"));
	}

	It(send_and_receive_play_midi_machine_control_message) {
		PhMidiInput midiIn;
		PhMidiOutput midiOut;

		int playCount = 0;

		QObject::connect(&midiIn, &PhMidiInput::play, [&]() {
			playCount++;
		});

		Assert::That(midiIn.open("testMMCStop"));
		Assert::That(midiOut.open("testMMCStop"));

		Assert::That(playCount, Equals(0));

		midiOut.sendMMCPlay();
		QThread::msleep(10);

		Assert::That(playCount, Equals(1));
	}

	It(send_and_receive_stop_midi_machine_control_message) {
		PhMidiInput midiIn;
		PhMidiOutput midiOut;

		int stopCount = 0;

		QObject::connect(&midiIn, &PhMidiInput::stop, [&]() {
			stopCount++;
		});

		Assert::That(midiIn.open("testMMCStop"));
		Assert::That(midiOut.open("testMMCStop"));

		Assert::That(stopCount, Equals(0));

		midiOut.sendMMCStop();
		QThread::msleep(10);

		Assert::That(stopCount, Equals(1));
	}

	It(send_and_receive_goto_midi_machine_control_message) {
		PhMidiInput midiIn;
		PhMidiOutput midiOut;

		int tcCount = 0;
		PhTime time = 0;
		PhTimeCodeType tcType = PhTimeCodeType25;

		QObject::connect(&midiIn, &PhMidiInput::timeCodeReceived, [&](int h, int m, int s, int f, PhTimeCodeType type) {
			tcCount++;
			tcType = type;
			time = PhTimeCode::timeFromHhMmSsFf(h, m, s, f, tcType);
		});

		Assert::That(midiIn.open("testMMCGoto"));
		Assert::That(midiOut.open("testMMCGoto"));

		Assert::That(tcCount, Equals(0));
		Assert::That(tcType, Equals(PhTimeCodeType25));

		midiOut.sendMMCGoto(2, 3, 4, 5, PhTimeCodeType24);
		QThread::msleep(10);

		Assert::That(tcCount, Equals(1));
		Assert::That(tcType, Equals(PhTimeCodeType24));
		Assert::That(t2s(time, tcType), Equals("02:03:04:05"));
	}

	It(update_a_clock_from_midi_timecode_message) {
		//
		// Initialize the midi timecode reader:
		//

		PhMidiTimeCodeReader mtcReader(PhTimeCodeType25);
		PhMidiOutput midiOut;

		PhTimeCodeType tcType = PhTimeCodeType25;
		int tcTypeCalled = 0;
		QObject::connect(&mtcReader, &PhMidiTimeCodeReader::timeCodeTypeChanged, [&](PhTimeCodeType type) {
			tcType = type;
			tcTypeCalled++;
		});

		Assert::That(mtcReader.open("testMTCReader"));
		Assert::That(midiOut.open("testMTCReader"));

		midiOut.sendFullTC(1, 0, 0, 0, PhTimeCodeType24);
		QThread::msleep(10);
		Assert::That(tcTypeCalled, Equals(1));
		Assert::That(tcType, Equals(PhTimeCodeType24));
		Assert::That(mtcReader.timeCodeType(), Equals(PhTimeCodeType24));
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("01:00:00:00"));

		//
		// Any quarter frame message should trigger play mode
		//

		midiOut.sendQFTC(0x02); // Send frame low digit
		QThread::msleep(10);

		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("01:00:00:00"));
		Assert::That(mtcReader.clock()->rate(), EqualsWithDelta(1, 0.001f));

		// Test basic playback behaviour

		midiOut.sendQFTC(0x10); // Send frame high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x20); // Send second low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x30); // Send second high digit
		QThread::msleep(10);

		// Since 4 quarter frame message have elapsed the frame increment by one
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("01:00:00:01"));

		midiOut.sendQFTC(0x40); // Send minute low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x50); // Send minute high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x61); // Send hour low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x70); // Send hour high digit and 24 fps info
		QThread::msleep(10);

		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("01:00:00:02"));

		// Send 8 quarter frame message from another timecode (23:40:19:20)

		midiOut.sendQFTC(0x06); // Send frame low digit (6 because we start the next frame transmission)
		QThread::msleep(10);
		midiOut.sendQFTC(0x11); // Send frame high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x23); // Send second low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x31); // Send second high digit
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("01:00:00:03"));
		midiOut.sendQFTC(0x48); // Send minute low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x52); // Send minute high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x67); // Send hour low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x71); // Send hour high digit and 24 fps info
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("23:40:19:22"));

		// Send the next 8 quarter frame message to check passing seconds

		midiOut.sendQFTC(0x00); // Send frame low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x10); // Send frame high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x24); // Send second low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x31); // Send second high digit
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("23:40:19:23"));
		midiOut.sendQFTC(0x48); // Send minute low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x52); // Send minute high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x67); // Send hour low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x71); // Send hour high digit and 24 fps info
		QThread::msleep(10);

		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("23:40:20:00"));

		// Test passing minutes (from 10:03:59:20 to 10:04:00:00)

		midiOut.sendQFTC(0x06); // Send frame low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x11); // Send frame high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x2b); // Send second low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x33); // Send second high digit
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("23:40:20:01"));
		midiOut.sendQFTC(0x43); // Send minute low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x50); // Send minute high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x6a); // Send hour low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x70); // Send hour high digit and 24 fps info
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("10:03:59:22"));
		midiOut.sendQFTC(0x00); // Send frame low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x10); // Send frame high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x20); // Send second low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x30); // Send second high digit
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("10:03:59:23"));
		midiOut.sendQFTC(0x44); // Send minute low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x50); // Send minute high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x6a); // Send hour low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x70); // Send hour high digit and 24 fps info
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("10:04:00:00"));
		midiOut.sendQFTC(0x02); // Send frame low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x10); // Send frame high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x20); // Send second low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x30); // Send second high digit
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("10:04:00:01"));
		midiOut.sendQFTC(0x44); // Send minute low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x50); // Send minute high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x6a); // Send hour low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x70); // Send hour high digit and 24 fps info
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("10:04:00:02"));

		// Switch to 25 fps timecode
		Assert::That(tcTypeCalled, Equals(1));
		Assert::That(tcType, Equals(PhTimeCodeType24));

		midiOut.sendQFTC(0x04); // Send frame low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x10); // Send frame high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x20); // Send second low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x30); // Send second high digit
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("10:04:00:03"));
		midiOut.sendQFTC(0x44); // Send minute low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x50); // Send minute high digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x6a); // Send hour low digit
		QThread::msleep(10);
		midiOut.sendQFTC(0x72); // Send hour high digit and 25fps info
		QThread::msleep(10);

		Assert::That(tcTypeCalled, Equals(2));
		Assert::That(tcType, Equals(PhTimeCodeType25));
		Assert::That(mtcReader.timeCodeType(), Equals(PhTimeCodeType25));
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType25), Equals("10:04:00:04"));

		// Stop sending quarter frame MTC message should stop the reader after one frame:
#warning /// @todo QThread::msleep block the pause detector timer and QTest::qWait crashes...
		//	QThread::msleep(30);
		//	Assert::That(mtcReader.clock()->rate(), EqualsWithDelta(1, 0.0001f));
		//	QThread::msleep(200);
		//	Assert::That(mtcReader.clock()->rate(), EqualsWithDelta(0, 0.0001f));
	}

	It(write_midi_timecode_message_from_a_clock) {
		PhMidiTimeCodeWriter mtcWriter(PhTimeCodeType30);
		PhMidiInput midiIn;

		int quarterFrameCount = 0;
		unsigned char quarterFrameData = 255;

		QObject::connect(&midiIn, &PhMidiInput::quarterFrame, [&](unsigned char data) {
			quarterFrameCount++;
			quarterFrameData = data;
		});

		Assert::That(midiIn.open("testMTCWriter"));
		Assert::That(mtcWriter.open("testMTCWriter"));

		mtcWriter.clock()->setTime(s2t("23:40:19:16", PhTimeCodeType30));
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(0));

		mtcWriter.clock()->setRate(1);
		PhFrame freq = PhTimeCode::getFps(PhTimeCodeType30) * 4; // => one quarter frame frequency
		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(1));
		Assert::That(quarterFrameData, Equals(0x02));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(2));
		Assert::That(quarterFrameData, Equals(0x11));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(3));
		Assert::That(quarterFrameData, Equals(0x23));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(4));
		Assert::That(quarterFrameData, Equals(0x31));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(5));
		Assert::That(quarterFrameData, Equals(0x48));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(6));
		Assert::That(quarterFrameData, Equals(0x52));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(7));
		Assert::That(quarterFrameData, Equals(0x67));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(mtcWriter.clock()->time(), Equals(s2t("23:40:19:18", PhTimeCodeType30)));
		Assert::That(quarterFrameCount, Equals(8));
		Assert::That(quarterFrameData, Equals(0x70 | (0x03 << 1) | 0x01)); // timecode type info + hour high digit

		// Test changing the writer timecode type:
		mtcWriter.setTimeCodeType(PhTimeCodeType25);
		freq = PhTimeCode::getFps(PhTimeCodeType25) * 4; // => one quarter frame frequency

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(9));
		Assert::That(quarterFrameData, Equals(0x01)); // With 25 fps, the next timecode will be 23:40:19:17

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(10));
		Assert::That(quarterFrameData, Equals(0x11));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(11));
		Assert::That(quarterFrameData, Equals(0x23));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(12));
		Assert::That(quarterFrameData, Equals(0x31));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(13));
		Assert::That(quarterFrameData, Equals(0x48));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(14));
		Assert::That(quarterFrameData, Equals(0x52));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(quarterFrameCount, Equals(15));
		Assert::That(quarterFrameData, Equals(0x67));

		mtcWriter.clock()->tick(freq);
		QThread::msleep(10);

		Assert::That(mtcWriter.clock()->time(), Equals(s2t("23:40:19:17", PhTimeCodeType25)));
		Assert::That(quarterFrameCount, Equals(16));
		Assert::That(quarterFrameData, Equals(0x70 | (0x01 << 1) | 0x01)); // timecode type info + hour high digit

		// Test changing speed to 0
		mtcWriter.clock()->setRate(0);

		// No quarter frame message shall be sent anymore
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(16));
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(16));
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(16));
		QThread::msleep(10);
		Assert::That(quarterFrameCount, Equals(16));


	}

	It(update_a_clock_from_midi_timecode_message_and_mmc_message) {
		//
		// Initialize the midi timecode reader:
		//

		PhMidiTimeCodeReader mtcReader(PhTimeCodeType25);
		PhMidiOutput midiOut;

		PhTimeCodeType tcType = PhTimeCodeType25;

		Assert::That(mtcReader.open("testMMCReader"));
		Assert::That(midiOut.open("testMMCReader"));

		// Send goto
		midiOut.sendMMCGoto(1, 0, 0, 0, PhTimeCodeType25);
		QThread::msleep(10);
		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType24), Equals("01:00:00:00"));

		// Send play
		midiOut.sendMMCPlay();
		QThread::msleep(10);
		Assert::That(mtcReader.clock()->rate(), EqualsWithDelta(1, 0.0001f));

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

		Assert::That(t2s(mtcReader.clock()->time(), PhTimeCodeType25), Equals("01:00:00:02"));

		// Send stop
		midiOut.sendMMCStop();
		QThread::msleep(10);
		Assert::That(mtcReader.clock()->rate(), EqualsWithDelta(0, 0.0001f));

		// Make sur quarter frame message doesn't put play back for the next 80 ms

		midiOut.sendQFTC(0x04); // Send frame low digit
		QThread::msleep(10);
		Assert::That(mtcReader.clock()->rate(), EqualsWithDelta(0, 0.0001f));

		midiOut.sendQFTC(0x10); // Send frame high digit
		QThread::msleep(60);
		Assert::That(mtcReader.clock()->rate(), EqualsWithDelta(0, 0.0001f));

		midiOut.sendQFTC(0x20); // Send second low digit
		QThread::msleep(20);
		Assert::That(mtcReader.clock()->rate(), EqualsWithDelta(1, 0.0001f));
	}
};

