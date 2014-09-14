/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhLtcReader.h"

PhLtcReader::PhLtcReader(PhLtcReaderSettings *settings) :
	_settings(settings),
	_tcType((PhTimeCodeType) settings->ltcReaderTimeCodeType()),
	_position(0),
	_noFrameCounter(0),
	_lastFrameDigit(0),
	_oldLastFrameDigit(0),
	_badTimeCodeGapCounter(0)
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

		if(_settings->ltcAutoDetectTimeCodeType()) {
			// If the frame is xx:xx:xx:00 ie, the previous frame was
			// the biggest one (23 for 24fps...)
			if(stime.frame == 0) {
				// If the old last digit is the same than the last frame digit
				// the counter goes up (it's a confirmation of the change
				if(_oldLastFrameDigit == _lastFrameDigit)
					_badTimeCodeGapCounter++;
				// If the old last frame digit is different than the last
				// frame digit, the tcType might have changed so the
				// counter is reset
				else
					_badTimeCodeGapCounter = 0;

				// If the old last digit is the same than the last digit
				// for 5 consecutive time, we update the tcType
				if(_badTimeCodeGapCounter >= 5) {
					if(_lastFrameDigit == 23) {
						updateTCType(PhTimeCodeType24);
					}
					else if(_lastFrameDigit == 24) {
						updateTCType(PhTimeCodeType25);
					}
					else {
						updateTCType(PhTimeCodeType30);
					}
				}

				_oldLastFrameDigit = _lastFrameDigit;
			}

			_lastFrameDigit = stime.frame;
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
		_badTimeCodeGapCounter = 0;
		emit timeCodeTypeChanged(tcType);
	}
}
