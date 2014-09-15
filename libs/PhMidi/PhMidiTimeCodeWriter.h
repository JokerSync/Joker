/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDITIMECODEWRITER_H
#define PHMIDITIMECODEWRITER_H

#include "PhSync/PhClock.h"

#include "PhMidiOutput.h"

/**
 * @brief PhMidiTimeCodeWriter send midi time code message
 *
 * This class can open an existing midi port and send
 * midi timecode messages according to its clock.
 */
class PhMidiTimeCodeWriter : public PhMidiOutput
{
	Q_OBJECT
public:
	/**
	 * @brief The PhMidiTimeCodeWriter constructor
	 * @param tcType The initial clock timecode type.
	 */
	PhMidiTimeCodeWriter(PhTimeCodeType tcType);

	/**
	 * @brief The timecode type used to write MTC
	 * @return A timecode type value
	 */
	PhTimeCodeType timeCodeType();

	/**
	 * @brief Set the timecode type used to write MTC
	 * @param tcType A timecode type value
	 */
	void setTimeCodeType(PhTimeCodeType tcType);

	/**
	 * @brief The PhMidiTimeCodeWriter clock
	 *
	 * Manipulate this clock to send midi timecode
	 * message accordingly.
	 *
	 * @return A clock instance.
	 */
	PhClock *clock() {
		return &_clock;
	}

	/**
	 * @brief Send a MMC goto message
	 * @param time A time value
	 */
	void sendMMCGotoFromTime(PhTime time);

private slots:
	void onTimeChanged(PhTime time);

private:
	PhTimeCodeType _tcType;
	PhClock _clock;
	int _currentDigit;
};

#endif // PHMIDITIMECODEWRITER_H
