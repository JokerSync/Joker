#include "PhTools/PhDebug.h"
#include "PhAudioInput.h"

PhAudioInput::PhAudioInput(QObject *parent)
	: PhAudio(parent)
{
}

bool PhAudioInput::init(QString deviceName)
{
	if(!PhAudio::init(deviceName)) {
		return false;
	}

	int deviceCount = Pa_GetDeviceCount();

	PaStreamParameters streamParameters;
	streamParameters.device = Pa_GetDefaultInputDevice();
	const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(streamParameters.device);
	streamParameters.channelCount = 1;
	streamParameters.sampleFormat = paInt16; //paUInt8 does not work on Windows (samples all zero)
	//24 fps => 41.6 ms per frame => we ask for 20 ms latency
	//20 ms @ 48000 Hz => 960 samples => we will work on buffers of 512 samples
	//Note: zero latency does not work on Windows (overflows permanently)
	streamParameters.suggestedLatency = 0.020;
	streamParameters.hostApiSpecificStreamInfo = NULL;

	bool isThereInput = false;
	bool deviceFound = false;

	for(int i = 0; i < deviceCount; i++ ) {
		deviceInfo = Pa_GetDeviceInfo( i );
		if(deviceInfo->maxInputChannels > 0 ) {
			isThereInput = true;
			if(deviceName == QString::fromLatin1(deviceInfo->name)) {
				deviceFound = true;
				streamParameters.device = i;
				deviceInfo = Pa_GetDeviceInfo(i);
				break;
			}
		}
	}
	if(!isThereInput) {
		PHDBG(0) << "No output device";
		return false;
	}
	if(deviceName.length() and !deviceFound) {
		PHDBG(0) << "Desired input not found :" << deviceName;
		return false;
	}

	PHDBG(0) << "Opening " << deviceInfo->name;

#warning /// @todo use the settings for sample rate and frame per buffer
	PaError err = Pa_OpenStream(&_stream, &streamParameters, NULL, 48000, 512, paNoFlag, audioCallback, this);
	if(err != paNoError) {
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	if(err != paNoError)
		return false;

	if(Pa_StartStream( _stream ) != paNoError)
		return false;

	PHDEBUG << deviceInfo->name << "is now open.";

	return true;
}

QList<QString> PhAudioInput::inputList()
{
	Pa_Initialize();
	QList<QString> names;
	int numDevices = Pa_GetDeviceCount();
	if( numDevices <= 0 )
		PHDBG(21) << "ERROR: Pa_CountDevices returned " << numDevices;
	else {
		const PaDeviceInfo *deviceInfo;
		for(int i = 0; i < numDevices; i++ ) {
			deviceInfo = Pa_GetDeviceInfo( i );
			if(deviceInfo->maxInputChannels > 0)
				names.append(QString::fromLatin1(deviceInfo->name));
		}
	}
	Pa_Terminate();

	return names;
}

int PhAudioInput::processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer)
{
	const short *buffer = (short*) inputBuffer;

	int minLevel = 0;
	int maxLevel = 0;
	for(unsigned long i = 0; i < framesPerBuffer; i++) {
		if(buffer[i] < minLevel)
			minLevel = buffer[i];
		if(buffer[i] > maxLevel)
			maxLevel = buffer[i];
	}

	emit audioProcessed(minLevel, maxLevel);

	return paContinue;
}
