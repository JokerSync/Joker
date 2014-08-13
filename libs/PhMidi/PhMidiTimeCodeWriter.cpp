/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhMidiTimeCodeWriter.h"

PhMidiTimeCodeWriter::PhMidiTimeCodeWriter(PhTimeCodeType tcType) :
	_clock(tcType), _currentDigit(0)
{
	connect(&_clock, &PhClock::timeChanged, this, &PhMidiTimeCodeWriter::onTimeChanged);
}

void PhMidiTimeCodeWriter::onTimeChanged(PhTime time)
{
	if(_clock.rate() == 1) {
		unsigned int hhmmssff[4];
		PhTimeCodeType tcType = _clock.timeCodeType();
		PhTimeCode::ComputeHhMmSsFfFromTime(hhmmssff, time, tcType);
		unsigned char data = _currentDigit << 4;
		switch (_currentDigit) {
		case 0:
			data |= hhmmssff[3] & 0x0F;
			break;
		case 1:
			data |= (hhmmssff[3] & 0xF0) >> 4;
			break;
		case 2:
			data |= hhmmssff[2] & 0x0F;
			break;
		case 3:
			data |= (hhmmssff[2] & 0xF0) >> 4;
			break;
		case 4:
			data |= hhmmssff[1] & 0x0F;
			break;
		case 5:
			data |= (hhmmssff[1] & 0xF0) >> 4;
			break;
		case 6:
			data |= hhmmssff[0] & 0x0F;
			break;
		case 7:
			data |= computeH(hhmmssff[0], tcType);
			break;
		}
		this->sendQFTC(data);
		_currentDigit = (_currentDigit + 1) % 8;
	}
}
