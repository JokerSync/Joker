#include "PhTools/PhDebug.h"
#include "AudioTestReader.h"

AudioTestReader::AudioTestReader(QObject *parent) :
	PhAudioReader(parent)
{
}

int AudioTestReader::processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer)
{
	const char *buffer = (const char*)inputBuffer;

	int min = 0;
	int max = 0;
	for(int i = 0; i < framesPerBuffer; i++) {
		if(buffer[i] < min)
			min = buffer[i];
		if(buffer[i] > max)
			max = buffer[i];
	}
	return paContinue;
}
