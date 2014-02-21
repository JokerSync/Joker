#ifndef AUDIOTESTREADER_H
#define AUDIOTESTREADER_H

#include "PhAudio/PhAudioInput.h"

class AudioTestReader : public PhAudioInput
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
