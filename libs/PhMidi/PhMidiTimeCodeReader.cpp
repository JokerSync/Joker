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

void PhMidiTimeCodeReader::onQuarterFrame(unsigned char data)
{
	_clock.setRate(1);
	_clock.tick(4 * PhTimeCode::getFps(_clock.timeCodeType()));

	unsigned int hhmmssff[4];
	PhTimeCode::ComputeHhMmSsFfFromTime(hhmmssff, _clock.time(), _clock.timeCodeType());

	bool change = false;
	PhTimeCodeType tcType = _clock.timeCodeType();

	switch (data >> 4) {
	case 3:
		if((hhmmssff[3] != _ff) || (hhmmssff[2] != _ss)) {
			hhmmssff[3] = _ff;
			hhmmssff[2] = _ss;
			change = true;
		}
		break;
	case 7:
		tcType = computeTimeCodeType((data & 0x0f) >> 1);
		if((hhmmssff[1] != _mm) || (hhmmssff[0] != _hh) || (_clock.timeCodeType() != tcType)) {
			hhmmssff[1] = _mm;
			hhmmssff[0] = _hh;
			change = true;
		}
	default:
		break;
	}

	if(change) {
		PHDEBUG << hhmmssff[0] << hhmmssff[1] << hhmmssff[2] <<hhmmssff[3];
		_clock.setTimeCodeType(tcType);
		_clock.setTime(PhTimeCode::timeFromHhMmSsFf(hhmmssff, tcType));
	}
}

void PhMidiTimeCodeReader::onTimeCode(int hh, int mm, int ss, int ff, PhTimeCodeType tcType)
{
	PhTime time = PhTimeCode::timeFromHhMmSsFf(hh, mm, ss, ff, tcType);

	_clock.setTimeCodeType(tcType);
	_clock.setTime(time);
}
