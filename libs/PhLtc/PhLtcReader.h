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

#include "PhSync/PhClock.h"
#include "PhSync/PhTimeCode.h"

#include "PhAudio/PhAudioInput.h"

/**
 * @brief A synchronisation module via the LTC protocol
 */
class PhLtcReader : public PhAudioInput
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

protected:
	int processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer);

private:
	PhClock _clock;

	ltc_off_t _position;
	LTCDecoder * _decoder;
	/** @brief Used to detect pause in LTC signal */
	int _noFrameCounter;

};

#endif // PHLTCREADER_H
