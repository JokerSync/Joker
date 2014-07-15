/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhLtcReader.h"

PhLtcReader::PhLtcReader(PhTimeCodeType tcType, QObject *parent) :
	PhAudioInput(parent),
	_clock(tcType),
	_position(0),
	_noFrameCounter(0)
{
#warning /// @todo autodetect tc type
	_decoder = ltc_decoder_create(1920, 1920 * 2);
	PHDBG(21) << "LTC Reader created";
}

PhClock *PhLtcReader::clock()
{
	return &_clock;
}

int PhLtcReader::processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer)
{
	ltc_decoder_write_s16(_decoder, (short*)inputBuffer, framesPerBuffer, _position);
	LTCFrameExt frame;
	unsigned int hhmmssff[4];
	SMPTETimecode stime;
	PhFrame oldFrame = _clock.frame();
	while(ltc_decoder_read(_decoder, &frame)) {
		ltc_frame_to_time(&stime, &frame.ltc, 1);
		hhmmssff[0] = stime.hours;
		hhmmssff[1] = stime.mins;
		hhmmssff[2] = stime.secs;
		hhmmssff[3] = stime.frame;

		PhFrame newFrame = PhTimeCode::frameFromHhMmSsFf(hhmmssff, _clock.timeCodeType());
		PHDBG(20) << hhmmssff[0] << hhmmssff[1] << hhmmssff[2] << hhmmssff[3];

		if(newFrame > oldFrame)
			_clock.setRate(1);
		else if(newFrame < oldFrame )
			_clock.setRate(-1);
		else
			_clock.setRate(0);
		_clock.setFrame(newFrame);
		_noFrameCounter = 0;
	}

	_position += framesPerBuffer;

	_noFrameCounter++;
	if(_noFrameCounter > 20)
		_clock.setRate(0);

	return PhAudioInput::processAudio(inputBuffer, NULL, framesPerBuffer);
}
