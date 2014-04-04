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
	_decoder = ltc_decoder_create(1920, 1920 * 2);
	PHDBG(21) << "LTC Reader created";
}

PhClock *PhLtcReader::clock()
{
	return &_clock;
}

int PhLtcReader::processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer)
{
	int result = PhAudioInput::processAudio(inputBuffer, NULL, framesPerBuffer);

	ltc_decoder_write(_decoder, (ltcsnd_sample_t*)inputBuffer, framesPerBuffer, _position);
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

	return result;
}
