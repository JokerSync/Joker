/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHLTCWRITER_H
#define PHLTCWRITER_H

#include <ltc.h>

#include "PhTools/PhClock.h"
#include "PhTools/PhTimeCode.h"

#include "PhAudio/PhAudioWriter.h"

/**
 * @brief Send master LTC generator
 */
class PhLtcWriter : public PhAudioWriter
{
	Q_OBJECT
public:
	/**
	 * @brief PhLtcWriter constructor
	 * @param tcType the timecode type
	 * @param parent the reader's parent
	 */
	explicit PhLtcWriter(PhTimeCodeType tcType, QObject *parent = 0);
	/**
	 * @brief Get the writer clock
	 * @return The writer clock
	 */
	PhClock *clock();

private:

	int processAudio(const void *, void *outputBuffer, unsigned long framesPerBuffer);

	PhClock _clock;
	LTCEncoder *_encoder;
	SMPTETimecode _st;

};

#endif // PHLTCWRITER_H
