#ifndef PHAUDIOOUTPUT_H
#define PHAUDIOOUTPUT_H

#include "PhAudio.h"

/**
 * @brief A generic audio output device
 *
 * Initialize an audio input device. The child must provide an implementation
 * for the processAudio() method.
 */
class PhAudioOutput : public PhAudio
{
public:

	/**
	 * @brief Initialize the output device
	 *
	 * It initialize the output device on the given name if it's found,
	 * or take the default output device if not.
	 * @param deviceName The desired output device name
	 * @return True if succeed, false otherwise
	 */
	bool init(QString deviceName);

	/**
	 * @brief Get the output list
	 * @return Return all the output devices
	 */
	static QList<QString> outputList();
};

#endif // PHAUDIOOUTPUT_H
