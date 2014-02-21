#ifndef PHAUDIOWRITER_H
#define PHAUDIOWRITER_H

#include "PhAudio.h"

class PhAudioWriter : public PhAudio
{
public:
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
