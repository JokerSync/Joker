#include "PhTools/PhDebug.h"
#include "PhAudioWriter.h"

PhAudioWriter::PhAudioWriter(QObject *parent)
	: PhAudio(parent)
{
}

bool PhAudioWriter::init(QString deviceName)
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

	PaStreamParameters outputDeviceInfo;
	outputDeviceInfo.device = Pa_GetDefaultOutputDevice();
	outputDeviceInfo.channelCount = 1;
	outputDeviceInfo.sampleFormat = paInt8;
	outputDeviceInfo.suggestedLatency = 0;
	outputDeviceInfo.hostApiSpecificStreamInfo = NULL;

	bool isThereOutput = false;
	bool deviceFound = false;

	for(int i = 0; i < deviceCount; i++ ) {
		const PaDeviceInfo *deviceInfo;
		deviceInfo = Pa_GetDeviceInfo( i );
		if(deviceInfo->maxOutputChannels > 0 ) {
			isThereOutput = true;
			if(deviceName == deviceInfo->name) {
				deviceFound = true;
				outputDeviceInfo.device = i;
				break;
			}
		}
	}
	if(!isThereOutput) {
		PHDBG(0) << "No output device";
		return false;
	}
	if(deviceName.length() and !deviceFound) {
		PHDBG(0) << "Desired output not found :" << deviceName;
		return false;
	}

#warning TODO use the settings for sample rate and frame per buffer
	PaError err = Pa_OpenStream(&_stream, NULL, &outputDeviceInfo, 48000, 1920, paNoFlag, audioCallback, this);

	if(err != paNoError) {
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	err = Pa_StartStream( _stream );
	if(err != paNoError) {
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	PHDEBUG << deviceName << "is now open.";

	return true;
}

QList<QString> PhAudioWriter::outputList()
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
				names.append(deviceInfo->name);
			}
		}
	}

	foreach(QString string, names) {
		PHDEBUG << string;
	}

	return names;
}
