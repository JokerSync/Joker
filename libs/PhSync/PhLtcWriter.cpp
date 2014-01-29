
#include <qendian.h>

#include "PhLtcWriter.h"

PhLtcWriter::PhLtcWriter(PhTimeCodeType tcType, QObject *parent) :
	QObject(parent),
	_clock(tcType),
	_encoder(NULL),
	_output(NULL),
	_buffer(0)
{

	_encoder = ltc_encoder_create(1, 1, LTC_TV_625_50, LTC_USE_DATE);
	switch (tcType) {
	case PhTimeCodeType25:
		ltc_encoder_set_bufsize(_encoder, _sampleRate, 25.0);
		//ltc_encoder_reinit(_encoder, _sampleRate, tcType, fps==25?LTC_TV_625_50:LTC_TV_525_60, LTC_USE_DATE);
		ltc_encoder_reinit(_encoder, _sampleRate, 25.0, LTC_TV_625_50, LTC_USE_DATE);
		break;
	case PhTimeCodeType24:
	case PhTimeCodeType2398:
		ltc_encoder_set_bufsize(_encoder, _sampleRate, 24.0);
		ltc_encoder_reinit(_encoder, _sampleRate, tcType, LTC_TV_525_60, LTC_USE_DATE);
		break;
	case PhTimeCodeType2997:
		ltc_encoder_set_bufsize(_encoder, _sampleRate, 29.97);
		ltc_encoder_reinit(_encoder, _sampleRate, tcType, LTC_TV_525_60, LTC_USE_DATE);
		break;
	default:
		break;
	}

	ltc_encoder_set_volume(_encoder, -18.0);


}

bool PhLtcWriter::init(QString output)
{
	QList<QAudioDeviceInfo> list = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

	if(list.isEmpty())
	{
		PHDEBUG << "No audio output device";
		return false;
	}

	QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();

	foreach(QAudioDeviceInfo device, list)
	{
		if(device.deviceName() == output)
			info = device;
	}

	PHDEBUG << "LTC output device :" << info.deviceName();

	QAudioFormat format;
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleRate(48000);
	format.setChannelCount(1);
	format.setSampleSize(8);
	format.setSampleType(QAudioFormat::SignedInt);

	if(!info.isFormatSupported(format))
	{
		PHDEBUG << "Unsupported audio format";
		return false;
	}

	_output = new QAudioOutput(info, format);
	_buffer = _output->start();

	return true;
}

void PhLtcWriter::close()
{
	ltc_encoder_free(_encoder);

}

PhClock *PhLtcWriter::clock()
{
	return &_clock;
}

void PhLtcWriter::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	unsigned int hhmmssff[4];
	PhTimeCode::ComputeHhMmSsFf(hhmmssff, frame, tcType);
	_st.hours = hhmmssff[0];
	_st.mins = hhmmssff[1];
	_st.secs = hhmmssff[2];
	_st.frame = hhmmssff[3];
	ltc_encoder_set_timecode(_encoder, &_st);

	PHDEBUG << _st.hours << _st.mins << _st.secs << _st.frame;

	int len;
	ltcsnd_sample_t *buf;

	ltc_encoder_encode_frame(_encoder);

	buf = ltc_encoder_get_bufptr(_encoder, &len, 1);
	if (len > 0) {
		_buffer->write(reinterpret_cast<const char*>(buf), len);
	}
}

