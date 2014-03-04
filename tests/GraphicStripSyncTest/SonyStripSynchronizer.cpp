#include "SonyStripSynchronizer.h"
#include "PhTools/PhDebug.h"

SonyStripSynchronizer::SonyStripSynchronizer()
{
}

void SonyStripSynchronizer::setSonyClock(PhClock *clock)
{
	_sonyClock = clock;
	connect(_sonyClock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onSonyFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_sonyClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onSonyRateChanged(PhRate)));
}

void SonyStripSynchronizer::setStripClock(PhClock *clock)
{
	_stripClock = clock;
	connect(_stripClock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onStripFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void SonyStripSynchronizer::onSonyFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(qAbs(_stripClock->frame() - frame) > 10) {
		PHDEBUG << "error :" << frame - _stripClock->frame();
		_settingStripFrame = true;
		_stripClock->setFrame(frame);
		_settingStripFrame = false;

	}
}

void SonyStripSynchronizer::onSonyRateChanged(PhRate rate)
{
	_stripClock->setRate(rate);
}

void SonyStripSynchronizer::onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(!_settingStripFrame) {
		if(qAbs(_sonyClock->frame() - frame) > 1) {
			PHDEBUG << "error :" << _sonyClock->frame() - frame;
	#warning TODO handle frame difference error
			_settingSonyFrame = true;
			_sonyClock->setFrame(frame);
			_settingSonyFrame = false;
		}
	}
}

void SonyStripSynchronizer::onStripRateChanged(PhRate rate)
{
	if(!_settingStripRate) {
		_settingSonyRate = true;
		if(_sonyClock)
			_sonyClock->setRate(rate);
		_settingSonyRate = false;
	}
}
