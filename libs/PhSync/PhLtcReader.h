/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHLTCREADER_H
#define PHLTCREADER_H

#include <QString>
#include <QObject>
#include <QTime>

#include <ltc.h>

#include "PhTools/PhClock.h"
#include "PhTools/PhTimeCode.h"

#include "PhAudio/PhAudioReader.h"

/**
 * @brief A synchronisation module via the LTC protocol
 */
class PhLtcReader : public PhAudioReader
{

	Q_OBJECT

public:
	/**
	 * @brief PhLtcReader constructor
	 * @param tcType the timecode type
	 * @param parent the reader's parent
	 */
	explicit PhLtcReader(PhTimeCodeType tcType = PhTimeCodeType25, QObject *parent = 0);

	/**
	 * @brief Get the reader clock
	 * @return The reader clock
	 */
	PhClock * clock();

private:

	int processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer);

	PhClock _clock;

	ltc_off_t _position;
	LTCDecoder * _decoder;
};

#endif // PHLTCREADER_H
