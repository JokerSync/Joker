/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QThread>

#include "PhTools/PhDebug.h"

#include "PhMidiTimeCodeReader.h"

PhMidiTimeCodeReader::PhMidiTimeCodeReader(PhTimeCodeType tcType) :
	_tcType(tcType),
	_pauseDetectionCounter(0)
{
	connect(&_pauseDetectionTimer, &QTimer::timeout, this, &PhMidiTimeCodeReader::checkPause);
	_pauseDetectionTimer.start(10);
}

void PhMidiTimeCodeReader::onQuarterFrame(unsigned char data)
{
	this->moveToThread(QThread::currentThread());
	_clock.setRate(1);
	_clock.tick(4 * PhTimeCode::getFps(_tcType));

	unsigned int hhmmssff[4];
	PhTimeCode::ComputeHhMmSsFfFromTime(hhmmssff, _clock.time(), _tcType);

	// We apply correction only on the last sequence message
	if ((data >> 4) == 7) {
		if(_tcType != _mtcType) {
			_tcType = _mtcType;
			emit timeCodeTypeChanged(_tcType);
		}
		if((hhmmssff[3] != _ff)
		   || (hhmmssff[2] != _ss)
		   || (hhmmssff[1] != _mm)
		   || (hhmmssff[0] != _hh)) {
			PHDEBUG << _hh << _mm << _ss << _ff;
			_clock.setTime(PhTimeCode::timeFromHhMmSsFf(_hh, _mm, _ss, _ff, _tcType));
		}
	}

	_pauseDetectionCounter = 0;
}

void PhMidiTimeCodeReader::onTimeCode(int hh, int mm, int ss, int ff, PhTimeCodeType tcType)
{
	PhTime time = PhTimeCode::timeFromHhMmSsFf(hh, mm, ss, ff, tcType);
	_tcType = tcType;
	_clock.setTime(time);
	emit timeCodeTypeChanged(_tcType);
}

void PhMidiTimeCodeReader::checkPause()
{
	PHDEBUG << _pauseDetectionCounter;
	_pauseDetectionCounter++;
	if(_pauseDetectionCounter >= 4) {
		PHDEBUG << "Pause detected";
		_clock.setRate(0);
	}
}
