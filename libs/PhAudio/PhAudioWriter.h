#ifndef PHAUDIOWRITER_H
#define PHAUDIOWRITER_H

#include "PhAudio.h"

/**
 * @brief A generic audio output device
 *
 * Initialize an audio input device. The child must provide an implementation
 * for the processAudio() method.
 */
class PhAudioWriter : public PhAudio
{
public:
	/**
	 * @brief PhAudioWriter constructor
	 * @param parent The parent object
	 */
	PhAudioWriter(QObject *parent = 0);

	/**
	 * @brief Initialize the writer
	 *
	 * It initialize the writer on the given input device if it's found,
	 * or take the default output device if not.
	 * @param deviceName The desired output device
	 * @return True if succeed, false otherwise
	 */
	bool init(QString deviceName);

	/**
	 * @brief Get the output list
	 * @return Return all the output devices
	 */
	static QList<QString> outputList();
};

#endif // PHAUDIOWRITER_H
