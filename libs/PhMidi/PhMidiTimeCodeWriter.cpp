/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhMidiTimeCodeWriter.h"

PhMidiTimeCodeWriter::PhMidiTimeCodeWriter(PhTimeCodeType tcType) :
	_clock(tcType), _lastFrame(-1000), _currentDigit(0)
{
	connect(&_clock, &PhClock::frameChanged, this, &PhMidiTimeCodeWriter::onFrameChanged);
}

void PhMidiTimeCodeWriter::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	unsigned int hhmmssff[4];
	PhTimeCode::ComputeHhMmSsFf(hhmmssff, frame, tcType);
	if(qAbs(frame - _lastFrame) > 1) {
		this->sendFullTC(hhmmssff[0], hhmmssff[1], hhmmssff[2], hhmmssff[3], tcType);
		_currentDigit = 0;
	}
	else switch (_currentDigit++) {
		case 0:
			this->sendQFTC(hhmmssff[3] & 0x0f);
			break;
		case 1:
			this->sendQFTC((hhmmssff[3] & 0x0f) >> 4);
			break;
		case 2:
			this->sendQFTC(hhmmssff[2] & 0x0f);
			break;
		case 3:
			this->sendQFTC((hhmmssff[2] & 0x0f) >> 4);
			break;
		case 4:
			this->sendQFTC(hhmmssff[1] & 0x0f);
			break;
		case 5:
			this->sendQFTC((hhmmssff[1] & 0x0f) >> 4);
			break;
		case 6:
			this->sendQFTC(hhmmssff[0] & 0x0f);
			break;
		case 7:
			this->sendQFTC(hhmmssff[0] | (tcType << 5));
			break;
		}

	_lastFrame = frame;
}
