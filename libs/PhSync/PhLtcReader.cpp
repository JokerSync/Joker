#include "PhLtcReader.h"

PhLtcReader::PhLtcReader(PhTimeCodeType tcType, QObject *parent) :
    QObject(parent),
    _clock(tcType),
	_position(0)
{
	_decoder = ltc_decoder_create(1920, 1920 * 2);
	PHDBG(21) << "LTC Reader created";
}

bool PhLtcReader::init(QString)
{
	PaError err = Pa_Initialize();
	if( err != paNoError )
		return false;

	PHDBG(0) <<"Port audio succeed initialization !";

	int numDevices;

	numDevices = Pa_GetDeviceCount();
	if( numDevices <= 0 )
	{
		PHDBG(21) << "ERROR: Pa_CountDevices returned " << numDevices;
		return false;
	}

	const PaDeviceInfo *deviceInfo;
	bool isThereOutput = false;
	int i = 0;
	while(i < numDevices and !isThereOutput)
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		isThereOutput = (deviceInfo->maxInputChannels > 0);
		i++;
	}
	if(!isThereOutput)
		return false;

	PaError errorOpening = Pa_OpenDefaultStream( &stream,
						  1,			/* mono input */
						  0,			/* no output */
						  paInt8,		/* 8 bits output */
						  SAMPLE_RATE,
						  FRAME_PER_BUFFER, /* frames per buffer, i.e. the number
												  of sample frames that PortAudio will
												  request from the callback. Many apps
												  may want to use
												  paFramesPerBufferUnspecified, which
												  tells PortAudio to pick the best,
												  possibly changing, buffer size.*/
						  audioCallback, /* this is your callback function */
						  this ); /*This is a pointer that will be passed to
														   your callback*/

	if(errorOpening != paNoError)
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

	else
	{
		const   PaDeviceInfo *deviceInfo;
		for(int i = 0; i<numDevices; i++ )
		{
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
	while(ltc_decoder_read(_decoder, &frame))
	{
		hhmmssff[0] = frame.ltc.hours_tens * 10 + frame.ltc.hours_units;
		hhmmssff[1] = frame.ltc.mins_tens * 10 + frame.ltc.mins_units;
		hhmmssff[2] = frame.ltc.secs_tens * 10 + frame.ltc.secs_units;
		hhmmssff[3] = frame.ltc.frame_tens * 10 + frame.ltc.frame_units;

		PhFrame newFrame = PhTimeCode::frameFromHhMmSsFf(hhmmssff, PhTimeCodeType25);
		PHDBG(20) << hhmmssff[0] << hhmmssff[1] << hhmmssff[2] << hhmmssff[3];

		if(oldFrame != newFrame)
		{
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

int PhLtcReader::audioCallback(const void * inputBuffer, void *, unsigned long , const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags , void *userData)
{
	PhLtcReader * LTCReader = (PhLtcReader *) userData;
	LTCReader->processAudio(inputBuffer, FRAME_PER_BUFFER);
	return 0;
}
