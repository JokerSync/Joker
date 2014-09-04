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

#include "PhLtcReaderSettings.h"

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
	explicit PhLtcReader(PhLtcReaderSettings * settings, PhTimeCodeType tcType = PhTimeCodeType25, QObject *parent = 0);

	/**
	 * @brief Get the reader clock
	 * @return The reader clock
	 */
	PhClock * clock();

	/**
	 * @brief The decoded timecode type by the LTC reader
	 * @return A timecode type value.
	 */
	PhTimeCodeType timeCodeType();

signals:
	/**
	 * @brief Emit a signal when the timecodeType change
	 * @param the new timecode type
	 */
	void tcTypeChanged(PhTimeCodeType tcType);

protected:
	int processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer);

private:	
	PhLtcReaderSettings * _settings;

	PhTimeCodeType _tcType;
	PhClock _clock;
	ltc_off_t _position;
	LTCDecoder * _decoder;
	/** @brief Used to detect pause in LTC signal */
	int _noFrameCounter;

	int _lastFrame;
	int _counter;
	int _oldLastFrame;

	void updateTCType(PhTimeCodeType tcType);
};

#endif // PHLTCREADER_H
