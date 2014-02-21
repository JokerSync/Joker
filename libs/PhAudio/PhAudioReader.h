#ifndef PHAUDIOREADER_H
#define PHAUDIOREADER_H

#include "PhAudio.h"

/**
 * @brief A generic audio input device
 *
 * Initialize an audio input device. The child must provide an implementation
 * for the processAudio() method.
 */
class PhAudioReader : public PhAudio
{
public:
	/**
	 * @brief PhAudioReader constructor
	 * @param parent The parent object
	 */
	PhAudioReader(QObject *parent = 0);

	/**
	 * @brief Initialize the reader
	 *
	 * It initialize the reader on the given input device if it's found,
	 * or take the default input device if not.
	 * @param deviceName The desired input device
	 * @return True if succeed, false otherwise
	 */
	bool init(QString deviceName);
	/**
	 * @brief Get the input list
	 * @return Return all the input devices
	 */
	static QList<QString> inputList();
};

#endif // PHAUDIOREADER_H
