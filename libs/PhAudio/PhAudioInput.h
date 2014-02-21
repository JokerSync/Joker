#ifndef PHAUDIOINPUT_H
#define PHAUDIOINPUT_H

#include "PhAudio.h"

/**
 * @brief A generic audio input device
 *
 * Initialize an audio input device. The child must provide an implementation
 * for the processAudio() method.
 */
class PhAudioInput : public PhAudio
{
public:
	/**
	 * @brief PhAudioInput constructor
	 * @param parent The parent object
	 */
	PhAudioInput(QObject *parent = 0);

	/**
	 * @brief Initialize the input device
	 *
	 * It initialize the input device on the given name if it's found,
	 * or take the default input device if not.
	 * @param deviceName The desired input device name
	 * @return True if succeed, false otherwise
	 */
	bool init(QString deviceName);
	/**
	 * @brief Get the input list
	 * @return Return all the input devices
	 */
	static QList<QString> inputList();
};

#endif // PHAUDIOINPUT_H
