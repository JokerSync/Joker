/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhLtcWriter.h"

PhLtcWriter::PhLtcWriter(PhTimeCodeType tcType) :
	PhAudioOutput(),
	_tcType(tcType),
	_encoder(NULL)
{
	_encoder = ltc_encoder_create(1, 1, LTC_TV_625_50, LTC_USE_DATE);
	// TODO fix this in the settings
	switch (tcType) {
	case PhTimeCodeType25:
		ltc_encoder_set_bufsize(_encoder, 48000, 25.0);
		//ltc_encoder_reinit(_encoder, 48000, tcType, fps==25?LTC_TV_625_50:LTC_TV_525_60, LTC_USE_DATE);
		ltc_encoder_reinit(_encoder, 48000, 25.0, LTC_TV_625_50, LTC_USE_DATE);
		break;
	case PhTimeCodeType24:
	case PhTimeCodeType2398:
		ltc_encoder_set_bufsize(_encoder, 48000, 24.0);
		ltc_encoder_reinit(_encoder, 48000, tcType, LTC_TV_525_60, LTC_USE_DATE);
		break;
	case PhTimeCodeType2997:
		ltc_encoder_set_bufsize(_encoder, 48000, 29.97);
		ltc_encoder_reinit(_encoder, 48000, tcType, LTC_TV_525_60, LTC_USE_DATE);
		break;
	case PhTimeCodeType30:
		ltc_encoder_set_bufsize(_encoder, 48000, 30);
		ltc_encoder_reinit(_encoder, 48000, tcType, LTC_TV_525_60, LTC_USE_DATE);
		break;
	default:
		break;
	}

	ltc_encoder_set_volume(_encoder, -18.0);

}

PhLtcWriter::~PhLtcWriter()
{
	ltc_encoder_free(_encoder);
}

PhClock *PhLtcWriter::clock()
{
	return &_clock;
}

int PhLtcWriter::processAudio(const void *, void *outputBuffer, unsigned long)
{
	unsigned int hhmmssff[4];
	PhTimeCode::ComputeHhMmSsFfFromTime(hhmmssff, _clock.time(), _tcType);
	SMPTETimecode st;
	st.hours = hhmmssff[0];
	st.mins = hhmmssff[1];
	st.secs = hhmmssff[2];
	st.frame = hhmmssff[3];
	ltc_encoder_set_timecode(_encoder, &st);

	PHDBG(21) << st.hours << st.mins << st.secs << st.frame;

	int len;
	ltcsnd_sample_t *buf;

	ltc_encoder_encode_frame(_encoder);

	buf = ltc_encoder_get_bufptr(_encoder, &len, 1);
	memcpy(outputBuffer, buf, len);
	_clock.elapse(PhTimeCode::timePerFrame(_tcType) / 4);

	return len;
}
