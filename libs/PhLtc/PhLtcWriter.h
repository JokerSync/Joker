/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHLTCWRITER_H
#define PHLTCWRITER_H

#include <ltc.h>

#include "PhSync/PhClock.h"
#include "PhSync/PhTimeCode.h"

#include "PhAudio/PhAudioOutput.h"

/**
 * @brief Send master LTC generator
 */
class PhLtcWriter : public PhAudioOutput
{
	Q_OBJECT
public:
	/**
	 * @brief PhLtcWriter constructor
	 * @param tcType the timecode type
	 */
	explicit PhLtcWriter(PhTimeCodeType tcType);

	/**
	 * @brief PhLtcWriter destructor
	 */
	~PhLtcWriter();

	/**
	 * @brief Get the writer clock
	 * @return The writer clock
	 */
	PhClock *clock();

private:

	int processAudio(const void *, void *outputBuffer, unsigned long);

	PhTimeCodeType _tcType;
	PhClock _clock;
	LTCEncoder *_encoder;
};

#endif // PHLTCWRITER_H
