/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhMidiTimeCodeReader.h"

PhMidiTimeCodeReader::PhMidiTimeCodeReader(PhTimeCodeType tcType) :
//	QObject(NULL),
	_clock(tcType)
{
//	PHDEBUG << connect(this, &PhMidiTimeCodeReader::timeCodeReceived, this, &PhMidiTimeCodeReader::onTC);
}

//bool PhMidiTimeCodeReader::open(QString portName)
//{
//	return _midiIn.open(portName);
//}

void PhMidiTimeCodeReader::onQuarterFrame()
{
	PHDEBUG;
	_clock.tick(4 * PhTimeCode::getFps(_clock.timeCodeType()));
}

void PhMidiTimeCodeReader::onTimeCode(int hh, int mm, int ss, int ff, PhTimeCodeType tcType)
{
	PhTime time = PhTimeCode::timeFromHhMmSsFf(hh, mm, ss, ff, tcType);

	_clock.setTime(time);
}
