#ifndef PHAUDIOREADER_H
#define PHAUDIOREADER_H

#include "PhAudio.h"

class PhAudioReader : public PhAudio
{
public:
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
