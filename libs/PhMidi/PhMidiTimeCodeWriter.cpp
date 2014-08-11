/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhMidiTimeCodeWriter.h"

PhMidiTimeCodeWriter::PhMidiTimeCodeWriter(PhTimeCodeType tcType) :
	_clock(tcType), _lastTime(-24000), _currentDigit(0)
{
	connect(&_clock, &PhClock::timeChanged, this, &PhMidiTimeCodeWriter::onTimeChanged);
}

void PhMidiTimeCodeWriter::onTimeChanged(PhTime time)
{
	unsigned int hhmmssff[4];
	PhTimeCodeType tcType = _clock.timeCodeType();
	PhTimeCode::ComputeHhMmSsFfFromTime(hhmmssff, time, tcType);
	if(qAbs(time - _lastTime) > PhTimeCode::timePerFrame(tcType) / 4) {
		this->sendFullTC(hhmmssff[0], hhmmssff[1], hhmmssff[2], hhmmssff[3], tcType);
		_currentDigit = 0;
	}
	else switch (_currentDigit++) {
		case 0:
			this->sendQFTC(0x00 | (hhmmssff[3] & 0x0f));
			break;
		case 1:
			this->sendQFTC(0x10 | ((hhmmssff[3] & 0x0f) >> 4));
			break;
		case 2:
			this->sendQFTC(0x20 | (hhmmssff[2] & 0x0f));
			break;
		case 3:
			this->sendQFTC(0x30 | ((hhmmssff[2] & 0x0f) >> 4));
			break;
		case 4:
			this->sendQFTC(0x40 | (hhmmssff[1] & 0x0f));
			break;
		case 5:
			this->sendQFTC(0x50 | ((hhmmssff[1] & 0x0f) >> 4));
			break;
		case 6:
			this->sendQFTC(0x60 | (hhmmssff[0] & 0x0f));
			break;
		case 7:
			this->sendQFTC(0x70 | (hhmmssff[0] | (tcType << 5)));
			break;
		}

	_lastTime = time;
}
