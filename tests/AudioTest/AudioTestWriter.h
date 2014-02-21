#ifndef AUDIOTESTWRITER_H
#define AUDIOTESTWRITER_H

#include "PhAudio/PhAudioWriter.h"

class AudioTestWriter : public PhAudioWriter
{
	Q_OBJECT
public:
	explicit AudioTestWriter(QObject *parent = 0);

signals:

public slots:

protected:
	int processAudio(const void *, void *outputBuffer, unsigned long framesPerBuffer);
};

#endif // AUDIOTESTWRITER_H
