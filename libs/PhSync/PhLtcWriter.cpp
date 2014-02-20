/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhLtcWriter.h"

PhLtcWriter::PhLtcWriter(PhTimeCodeType tcType, QObject *parent) :
	QObject(parent),
	_clock(tcType),
	_encoder(NULL)
{
	_encoder = ltc_encoder_create(1, 1, LTC_TV_625_50, LTC_USE_DATE);
	switch (tcType) {
	case PhTimeCodeType25:
		ltc_encoder_set_bufsize(_encoder, SAMPLE_RATE, 25.0);
		//ltc_encoder_reinit(_encoder, SAMPLE_RATE, tcType, fps==25?LTC_TV_625_50:LTC_TV_525_60, LTC_USE_DATE);
		ltc_encoder_reinit(_encoder, SAMPLE_RATE, 25.0, LTC_TV_625_50, LTC_USE_DATE);
		break;
	case PhTimeCodeType24:
	case PhTimeCodeType2398:
		ltc_encoder_set_bufsize(_encoder, SAMPLE_RATE, 24.0);
		ltc_encoder_reinit(_encoder, SAMPLE_RATE, tcType, LTC_TV_525_60, LTC_USE_DATE);
		break;
	case PhTimeCodeType2997:
		ltc_encoder_set_bufsize(_encoder, SAMPLE_RATE, 29.97);
		ltc_encoder_reinit(_encoder, SAMPLE_RATE, tcType, LTC_TV_525_60, LTC_USE_DATE);
		break;
	default:
		break;
	}

	ltc_encoder_set_volume(_encoder, -18.0);

}

bool PhLtcWriter::init(QString deviceName)
{
	PHDBG(0) << deviceName;

	PaError err = Pa_Initialize();
	if( err != paNoError )
		return false;

	PHDBG(0) <<"Port audio succeed initialization !";

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


	err = Pa_OpenStream(&stream, NULL, &outputDeviceInfo, SAMPLE_RATE, FRAME_PER_BUFFER, paNoFlag, audioCallback, this);

	if(err != paNoError) {
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	err = Pa_StartStream( stream );
	if(err != paNoError) {
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	return true;
}

void PhLtcWriter::close()
{
	Pa_CloseStream( stream );
}

QList<QString> PhLtcWriter::outputList()
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

PhClock *PhLtcWriter::clock()
{
	return &_clock;
}

int PhLtcWriter::processAudio(void *outputBuffer, unsigned long framesPerBuffer)
{
	unsigned int hhmmssff[4];
	PhTimeCode::ComputeHhMmSsFf(hhmmssff, _clock.frame(), _clock.timeCodeType());
	_st.hours = hhmmssff[0];
	_st.mins = hhmmssff[1];
	_st.secs = hhmmssff[2];
	_st.frame = hhmmssff[3];
	ltc_encoder_set_timecode(_encoder, &_st);

	PHDBG(21) << _st.hours << _st.mins << _st.secs << _st.frame;

	int len;
	ltcsnd_sample_t *buf;

	ltc_encoder_encode_frame(_encoder);

	buf = ltc_encoder_get_bufptr(_encoder, &len, 1);
	memcpy(outputBuffer, buf, len);
	_clock.tick(PhTimeCode::getFps(_clock.timeCodeType()));


	return len;
}

int PhLtcWriter::audioCallback(const void *, void *outputBuffer, unsigned long, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *userData)
{
	PhLtcWriter * LTCWriter = (PhLtcWriter *) userData;
	LTCWriter->processAudio(outputBuffer, FRAME_PER_BUFFER);
	return 0;
}
