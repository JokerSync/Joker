#include "PhTools/PhDebug.h"

#include "PhAudio.h"

PhAudio::PhAudio(QObject *parent) :
	QObject(parent),
	_stream(NULL),
	_paInitOk(false)
{
	PaError err = Pa_Initialize();
	if(err == paNoError) {
		PHDEBUG << "Port audio initialized:" << Pa_GetVersionText();
		_paInitOk = true;
	}
	else {
		PHDEBUG << Pa_GetErrorText(err);
	}
}

PhAudio::~PhAudio()
{
	if(_paInitOk) {
		close();
		Pa_Terminate();
	}
}

bool PhAudio::init(QString deviceName)
{
	return _paInitOk;
}

void PhAudio::close()
{
	if(_stream) {
		Pa_CloseStream( _stream );
		_stream = NULL;
	}
}

int PhAudio::audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *userData)
{
	PhAudio* audio = (PhAudio*)userData;
	return audio->processAudio(inputBuffer, outputBuffer, framesPerBuffer);
}
