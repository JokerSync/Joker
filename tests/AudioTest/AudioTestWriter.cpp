#include "PhTools/PhDebug.h"
#include "AudioTestWriter.h"

AudioTestWriter::AudioTestWriter(QObject *parent)
{
}

int AudioTestWriter::processAudio(const void *, void *outputBuffer, unsigned long framesPerBuffer)
{
	char *buffer = (char*)outputBuffer;
	int i = 0;
	int period = framesPerBuffer / 8;
	while (i < framesPerBuffer) {
		if((i % period) < (period / 2))
			buffer[i] = -10;
		else
			buffer[i] = 10;
		i++;
	}

	return paContinue;
}
