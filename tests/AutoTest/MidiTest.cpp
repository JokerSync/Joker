/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "MidiTest.h"

#include "PhMidi/PhMidiInput.h"
#include "PhMidi/PhMidiOutput.h"

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
