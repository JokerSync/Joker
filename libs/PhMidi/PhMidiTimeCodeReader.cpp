/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhMidiTimeCodeReader.h"

PhMidiTimeCodeReader::PhMidiTimeCodeReader(PhTimeCodeType tcType) :
	_clock(tcType)
{
}

void PhMidiTimeCodeReader::onQuarterFrame(unsigned char)
{
	_clock.tick(4 * PhTimeCode::getFps(_clock.timeCodeType()));
	_clock.setRate(1);
}

void PhMidiTimeCodeReader::onTimeCode(int hh, int mm, int ss, int ff, PhTimeCodeType tcType)
{
	PhTime time = PhTimeCode::timeFromHhMmSsFf(hh, mm, ss, ff, tcType);

	_clock.setTime(time);
}
