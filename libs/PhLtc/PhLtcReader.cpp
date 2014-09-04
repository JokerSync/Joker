/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhLtcReader.h"

PhLtcReader::PhLtcReader(PhLtcReaderSettings *settings, PhTimeCodeType tcType, QObject *parent) :
	PhAudioInput(parent),
	_tcType(tcType),
	_position(0),
	_noFrameCounter(0),
	_lastFrame(0),
	_settings(settings)
{
#warning /// @todo autodetect tc type
	_decoder = ltc_decoder_create(1920, 1920 * 2);
	PHDBG(21) << "LTC Reader created";
}

PhClock *PhLtcReader::clock()
{
	return &_clock;
}

PhTimeCodeType PhLtcReader::timeCodeType()
{
	return _tcType;
}

int PhLtcReader::processAudio(const void *inputBuffer, void *, unsigned long framesPerBuffer)
{
	ltc_decoder_write_s16(_decoder, (short*)inputBuffer, framesPerBuffer, _position);
	LTCFrameExt ltcFrame;
	unsigned int hhmmssff[4];
	SMPTETimecode stime;
	PhTime oldTime = _clock.time();
	while(ltc_decoder_read(_decoder, &ltcFrame)) {
		ltc_frame_to_time(&stime, &ltcFrame.ltc, 1);
		hhmmssff[0] = stime.hours;
		hhmmssff[1] = stime.mins;
		hhmmssff[2] = stime.secs;
		hhmmssff[3] = stime.frame;

		if(_settings->autoFPSDetection()) {
			PHDEBUG << "auto detect is on";
			if(stime.frame == 0) {
				if(_oldLastFrame == _lastFrame)
					_counter++;
				else
					_counter = 0;

				if(_counter >= 5) {
					if(_lastFrame == 23) {
						updateTCType(PhTimeCodeType24);
					}
					else if(_lastFrame == 24) {
						updateTCType(PhTimeCodeType25);
					}
					else {
						updateTCType(PhTimeCodeType30);
					}
				}
				_oldLastFrame = _lastFrame;
			}

			_lastFrame = stime.frame;
		}

		PhTime newTime = PhTimeCode::timeFromHhMmSsFf(hhmmssff, _tcType);
		PHDBG(20) << hhmmssff[0] << hhmmssff[1] << hhmmssff[2] << hhmmssff[3];

		if(newTime > oldTime)
			_clock.setRate(1);
		else if(newTime < oldTime )
			_clock.setRate(-1);
		else
			_clock.setRate(0);
		_clock.setTime(newTime);
		_noFrameCounter = 0;
	}

	_position += framesPerBuffer;

	_noFrameCounter++;
	if(_noFrameCounter > 20)
		_clock.setRate(0);

	return PhAudioInput::processAudio(inputBuffer, NULL, framesPerBuffer);
}

void PhLtcReader::updateTCType(PhTimeCodeType tcType)
{
	if(_tcType != tcType) {
		_tcType = tcType;
		_counter = 0;
		emit tcTypeChanged(tcType);
	}
}
