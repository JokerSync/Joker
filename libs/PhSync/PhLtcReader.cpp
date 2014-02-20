/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhLtcReader.h"

PhLtcReader::PhLtcReader(PhTimeCodeType tcType, QObject *parent) :
	QObject(parent),
	_clock(tcType),
	_position(0)
{
	_decoder = ltc_decoder_create(1920, 1920 * 2);
	PHDBG(21) << "LTC Reader created";
}

bool PhLtcReader::init(QString deviceName)
{
	PaError err = Pa_Initialize();
	if( err != paNoError )
		return false;

	PHDBG(0) <<"Port audio succeed initialization !";

	int deviceCount = Pa_GetDeviceCount();
	if( deviceCount <= 0 ) {
		PHDBG(21) << "ERROR: Pa_CountDevices returned " << deviceCount;
		return false;
	}


	PaStreamParameters streamParameters;
	streamParameters.device = Pa_GetDefaultOutputDevice();
	streamParameters.channelCount = 1;
	streamParameters.sampleFormat = paInt8;
	streamParameters.suggestedLatency = 0;
	streamParameters.hostApiSpecificStreamInfo = NULL;

	bool isThereInput = false;
	bool deviceFound = false;

	for(int i = 0; i < deviceCount; i++ ) {
		const PaDeviceInfo *deviceInfo;
		deviceInfo = Pa_GetDeviceInfo( i );
		if(deviceInfo->maxInputChannels > 0 ) {
			isThereInput = true;
			if(deviceName == deviceInfo->name) {
				deviceFound = true;
				streamParameters.device = i;
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

	err = Pa_OpenStream(&stream, &streamParameters, NULL, SAMPLE_RATE, FRAME_PER_BUFFER, paNoFlag, audioCallback, this);

	if(err != paNoError) {
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	if(err != paNoError)
		return false;

	if(Pa_StartStream( stream ) != paNoError)
		return false;

	return true;
}

void PhLtcReader::close()
{
	Pa_CloseStream( stream );
}

QList<QString> PhLtcReader::inputList()
{
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

	return names;
}

PhClock *PhLtcReader::clock()
{
	return &_clock;
}

int PhLtcReader::processAudio(const void *outputBuffer, unsigned long framesPerBuffer)
{

	ltc_decoder_write(_decoder, (ltcsnd_sample_t*)outputBuffer, framesPerBuffer, _position);
	LTCFrameExt frame;
	unsigned int hhmmssff[4];
	PhFrame oldFrame = _clock.frame();
	while(ltc_decoder_read(_decoder, &frame)) {
		hhmmssff[0] = frame.ltc.hours_tens * 10 + frame.ltc.hours_units;
		hhmmssff[1] = frame.ltc.mins_tens * 10 + frame.ltc.mins_units;
		hhmmssff[2] = frame.ltc.secs_tens * 10 + frame.ltc.secs_units;
		hhmmssff[3] = frame.ltc.frame_tens * 10 + frame.ltc.frame_units;

		PhFrame newFrame = PhTimeCode::frameFromHhMmSsFf(hhmmssff, PhTimeCodeType25);
		PHDBG(20) << hhmmssff[0] << hhmmssff[1] << hhmmssff[2] << hhmmssff[3];

		if(oldFrame != newFrame) {
			if(oldFrame < newFrame)
				_clock.setRate(1);
			else
				_clock.setRate(-1);
		}
		else
			_clock.setRate(0);
		// Handeling delay
		_clock.setFrame(newFrame);

	}

	_position += framesPerBuffer;

	return 0;
}

int PhLtcReader::audioCallback(const void * inputBuffer, void *, unsigned long, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *userData)
{
	PhLtcReader * LTCReader = (PhLtcReader *) userData;
	LTCReader->processAudio(inputBuffer, FRAME_PER_BUFFER);
	return 0;
}
