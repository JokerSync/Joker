/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhLtcWriter.h"

PhLtcWriter::PhLtcWriter(PhTimeCodeType tcType, QObject *parent) :
	PhAudioOutput(parent),
	_clock(tcType),
	_encoder(NULL)
{
	_encoder = ltc_encoder_create(1, 1, LTC_TV_625_50, LTC_USE_DATE);
#warning /// @todo fix this in the settings
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
	default:
		break;
	}

	ltc_encoder_set_volume(_encoder, -18.0);

}

PhClock *PhLtcWriter::clock()
{
	return &_clock;
}

int PhLtcWriter::processAudio(const void *, void *outputBuffer, unsigned long)
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
