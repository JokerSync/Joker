/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDITIMECODEWRITER_H
#define PHMIDITIMECODEWRITER_H

#include "PhSync/PhClock.h"

#include "PhMidiOutput.h"

class PhMidiTimeCodeWriter : public PhMidiOutput
{
	Q_OBJECT
public:
	PhMidiTimeCodeWriter(PhTimeCodeType tcType);

	PhClock *clock() {
		return &_clock;
	}

private slots:
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);

private:
	PhClock _clock;
	PhFrame _lastFrame;
	int _currentDigit;
};

#endif // PHMIDITIMECODEWRITER_H
