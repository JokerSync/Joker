#ifndef PHAUDIOREADER_H
#define PHAUDIOREADER_H

#include "PhAudio.h"

class PhAudioReader : public PhAudio
{
public:
	PhAudioReader(QObject *parent = 0);

	/**
	 * @brief Get the input list
	 * @return Return all the input devices
	 */
	static QList<QString> inputList();
};

#endif // PHAUDIOREADER_H
