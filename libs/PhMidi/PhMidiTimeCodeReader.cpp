/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhMidiTimeCodeReader.h"

PhMidiTimeCodeReader::PhMidiTimeCodeReader(PhTimeCodeType tcType) :
	_tcType(tcType),
	_lastStopDateTime(QDateTime::fromMSecsSinceEpoch(0)),
	_pauseDetectionCounter(0)
{
	connect(&_pauseDetectionTimer, &QTimer::timeout, this, &PhMidiTimeCodeReader::checkPause);
}

bool PhMidiTimeCodeReader::open(QString portName)
{
	if(PhMidiInput::open(portName)) {
		_pauseDetectionTimer.start(10);
		_lastStopDateTime = QDateTime::fromMSecsSinceEpoch(0);
		return true;
	}
	return false;
}

void PhMidiTimeCodeReader::close()
{
	_pauseDetectionTimer.stop();
	PhMidiInput::close();
}

void PhMidiTimeCodeReader::onQuarterFrame(unsigned char data)
{
	// Make sure that the last stop occured more than 80ms ago
	if((_lastStopDateTime.addMSecs(80) < QDateTime::currentDateTime()) && (_clock.rate() != 1)) {
		PHDBG(22) << "Play detected";
		_clock.setRate(1);
	}
	_clock.elapse(PhTimeCode::timePerFrame(_tcType) / 4);

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
			PHDBG(22) << _hh << _mm << _ss << _ff;
			_clock.setTime(PhTimeCode::timeFromHhMmSsFf(_hh, _mm, _ss, _ff, _tcType));
		}
	}

	_pauseDetectionCounter = 0;
}

void PhMidiTimeCodeReader::onTimeCode(int hh, int mm, int ss, int ff, PhTimeCodeType tcType)
{
	PhMidiInput::onTimeCode(hh, mm, ss, ff, tcType);
	PhTime time = PhTimeCode::timeFromHhMmSsFf(hh, mm, ss, ff, tcType);
	_clock.setTime(time);
	if(_tcType != tcType) {
		_tcType = tcType;
		emit timeCodeTypeChanged(_tcType);
	}
}

void PhMidiTimeCodeReader::onPlay()
{
	PHDBG(22);
	PhMidiInput::onPlay();
	_clock.setRate(1);
}

void PhMidiTimeCodeReader::onStop()
{
	PHDBG(22);
	PhMidiInput::onStop();
	_clock.setRate(0);
	_lastStopDateTime = QDateTime::currentDateTime();
}

void PhMidiTimeCodeReader::checkPause()
{
	_pauseDetectionCounter++;
	if(_pauseDetectionCounter >= 4) {
		if(_clock.rate() != 0) {
			PHDBG(22) << "Pause detected";
			_clock.setRate(0);
		}
	}
}
