#include "PhTools/PhDebug.h"
#include "PhAudioOutput.h"

bool PhAudioOutput::init(QString deviceName)
{
	PHDBG(0) << deviceName;

	if(!PhAudio::init(deviceName)) {
		return false;
	}

	int deviceCount = Pa_GetDeviceCount();
	if( deviceCount <= 0 ) {
		PHDBG(0) << "ERROR: Pa_CountDevices returned " << deviceCount;
		return false;
	}

	PaStreamParameters streamParameters;
	streamParameters.device = Pa_GetDefaultOutputDevice();
	streamParameters.channelCount = 1;
	streamParameters.sampleFormat = paInt8;
	streamParameters.suggestedLatency = 0;
	streamParameters.hostApiSpecificStreamInfo = NULL;

	bool isThereOutput = false;
	bool deviceFound = false;

	for(int i = 0; i < deviceCount; i++ ) {
		const PaDeviceInfo *deviceInfo;
		deviceInfo = Pa_GetDeviceInfo( i );
		if(deviceInfo->maxOutputChannels > 0 ) {
			isThereOutput = true;
			if(deviceName == QString::fromLatin1(deviceInfo->name)) {
				deviceFound = true;
				streamParameters.device = i;
				break;
			}
		}
	}
	if(!isThereOutput) {
		PHDBG(0) << "No output device";
		return false;
	}
	if(deviceName.length() && !deviceFound) {
		PHDBG(0) << "Desired output not found :" << deviceName;
		return false;
	}

	// TODO use the settings for sample rate and frame per buffer
	PaError err = Pa_OpenStream(&_stream, NULL, &streamParameters, 48000, 1920, paNoFlag, audioCallback, this);

	if(err != paNoError) {
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	err = Pa_StartStream( _stream );
	if(err != paNoError) {
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	PHDEBUG << Pa_GetDeviceInfo(streamParameters.device)->name << "is now open.";

	return true;
}

QList<QString> PhAudioOutput::outputList()
{
	QList<QString> names;
	int numDevices = Pa_GetDeviceCount();
	if( numDevices <= 0 )
		PHDBG(0) << "ERROR: Pa_CountDevices returned " << numDevices;
	else {
		const PaDeviceInfo *deviceInfo;
		for(int i = 0; i < numDevices; i++ ) {
			deviceInfo = Pa_GetDeviceInfo( i );
			if(deviceInfo->maxOutputChannels > 0) {
				//PHDEBUG << deviceInfo->name;
				names.append(QString::fromLatin1(deviceInfo->name));
			}
		}
	}

	foreach(QString string, names) {
		PHDEBUG << string;
	}

	return names;
}
