#include "PhTools/PhDebug.h"
#include "PhAudioReader.h"

PhAudioReader::PhAudioReader(QObject *parent)
	: PhAudio(parent)
{
}

QList<QString> PhAudioReader::inputList()
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
				names.append(deviceInfo->name);
		}
	}
	Pa_Terminate();

	return names;
}
