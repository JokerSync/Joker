#ifndef PHAUDIOWRITER_H
#define PHAUDIOWRITER_H

#include "PhAudio.h"

class PhAudioWriter : public PhAudio
{
public:
	PhAudioWriter(QObject *parent = 0);

	/**
	 * @brief Get the output list
	 * @return Return all the output devices
	 */
	static QList<QString> outputList();
};

#endif // PHAUDIOWRITER_H
