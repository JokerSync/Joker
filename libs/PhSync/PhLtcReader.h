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

#warning TODO put this in the settings
/** Number of frame processed by the audio callback call */
#define FRAME_PER_BUFFER 256
/** Audio sample rate */
#define SAMPLE_RATE 48000

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
	 * @brief Initialize the reader
	 *
	 * It initialize the reader on the given input device if it's found,
	 * or take the default input device if not.
	 * @param deviceName The desired input device
	 * @return True if succeed, false otherwise
	 */
	bool init(QString deviceName = "");

	/**
	 * @brief Get the input list
	 * @return Return all the input devices
	 */
	static QList<QString> inputList();
	/**
	 * @brief Get the reader clock
	 * @return The reader clock
	 */
	PhClock * clock();


private:

	int processAudio(const void *inputBuffer,
					 unsigned long framesPerBuffer);
	static int audioCallback( const void *inputBuffer, void *outputBuffer,
	                          unsigned long framesPerBuffer,
	                          const PaStreamCallbackTimeInfo* timeInfo,
	                          PaStreamCallbackFlags statusFlags,
	                          void *userData );

	PhClock _clock;

	float data;

	ltc_off_t _position;
	LTCDecoder * _decoder;
	int _pauseDetector;

};

#endif // PHLTCREADER_H
