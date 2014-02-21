#include "PhTools/PhDebug.h"
#include "PhAudioWriter.h"

PhAudioWriter::PhAudioWriter(QObject *parent)
	: PhAudio(parent)
{
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
