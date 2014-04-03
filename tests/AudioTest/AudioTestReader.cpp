#include "PhTools/PhDebug.h"
#include "AudioTestReader.h"

AudioTestReader::AudioTestReader(QObject *parent) :
	PhAudioInput(parent),
	_minLevel(0),
	_maxLevel(0)
{
}

int AudioTestReader::processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer)
{
	const char *buffer = (const char*)inputBuffer;

	_minLevel = 0;
	_maxLevel = 0;
	for(int i = 0; i < framesPerBuffer; i++) {
		if(buffer[i] < _minLevel)
			_minLevel = buffer[i];
		if(buffer[i] > _maxLevel)
			_maxLevel = buffer[i];
	}
	emit audioProcessed();
	return paContinue;
}
