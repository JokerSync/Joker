/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDITIMECODEREADER_H
#define PHMIDITIMECODEREADER_H

#include <QObject>

#include "PhSync/PhClock.h"

#include "PhMidiInput.h"

class PhMidiTimeCodeReader : public PhMidiInput
{
	Q_OBJECT
public:
	PhMidiTimeCodeReader(PhTimeCodeType tcType);

//	bool open(QString portName);
	PhClock *clock() {
		return &_clock;
	}

protected:
	void onQuarterFrame();
	void onTimeCode(int hh, int mm, int ss, int ff, PhTimeCodeType tcType);

private:
//j	PhMidiInput _midiIn;
	PhClock _clock;
};

#endif // PHMIDITIMECODEREADER_H
