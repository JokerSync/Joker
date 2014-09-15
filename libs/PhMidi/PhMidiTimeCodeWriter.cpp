/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhMidiTimeCodeWriter.h"

PhMidiTimeCodeWriter::PhMidiTimeCodeWriter(PhTimeCodeType tcType) :
	_tcType(tcType), _currentDigit(0)
{
	connect(&_clock, &PhClock::timeChanged, this, &PhMidiTimeCodeWriter::onTimeChanged);
}

PhTimeCodeType PhMidiTimeCodeWriter::timeCodeType()
{
	return _tcType;
}

void PhMidiTimeCodeWriter::setTimeCodeType(PhTimeCodeType tcType)
{
	_tcType = tcType;
}

void PhMidiTimeCodeWriter::sendMMCGotoFromTime(PhTime time)
{
	unsigned int hhmmssff[4];
	PhTimeCode::ComputeHhMmSsFfFromTime(hhmmssff, time, _tcType);
	sendMMCGoto((unsigned char)hhmmssff[0], (unsigned char)hhmmssff[1], (unsigned char)hhmmssff[2], (unsigned char)hhmmssff[3], _tcType);
}

void PhMidiTimeCodeWriter::onTimeChanged(PhTime time)
{
	if(_clock.rate() == 1) {
		unsigned int hhmmssff[4];

		// We add two frame in we are in the four quarter frame of
		// the message since we send the next frame tc
		if(_currentDigit < 4)
			time += 2 * PhTimeCode::timePerFrame(_tcType);

		PhTimeCode::ComputeHhMmSsFfFromTime(hhmmssff, time, _tcType);
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
			data |= computeH(hhmmssff[0], _tcType);
			break;
		}
		this->sendQFTC(data);
		_currentDigit = (_currentDigit + 1) % 8;
	}
}
