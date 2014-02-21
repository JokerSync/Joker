#ifndef AUDIOTESTREADER_H
#define AUDIOTESTREADER_H

#include "PhAudio/PhAudioReader.h"

class AudioTestReader : public PhAudioReader
{
	Q_OBJECT
public:
	explicit AudioTestReader(QObject *parent = 0);

signals:

public slots:

protected:
	int processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer);
};

#endif // AUDIOTESTREADER_H
