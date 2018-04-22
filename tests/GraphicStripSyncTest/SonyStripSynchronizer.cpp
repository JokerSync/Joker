#include "SonyStripSynchronizer.h"
#include "PhTools/PhDebug.h"

SonyStripSynchronizer::SonyStripSynchronizer()
{
}

void SonyStripSynchronizer::setSonyClock(PhClock *clock)
{
	_sonyClock = clock;
	connect(_sonyClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onSonyTimeChanged(PhFrame,PhTimeCodeType)));
	connect(_sonyClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onSonyRateChanged(PhRate)));
}

void SonyStripSynchronizer::setStripClock(PhClock *clock)
{
	_stripClock = clock;
	connect(_stripClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onStripTimeChanged(PhFrame,PhTimeCodeType)));
	connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void SonyStripSynchronizer::onSonyTimeChanged(PhTime time)
{
	if(qAbs(_stripClock->time() - time) > 10000) {
		PHDEBUG << "error :" << time - _stripClock->time();
		_settingStripTime = true;
		_stripClock->setTime(time);
		_settingStripTime = false;

	}
}

void SonyStripSynchronizer::onSonyRateChanged(PhRate rate)
{
	_stripClock->setRate(rate);
}

void SonyStripSynchronizer::onStripTimeChanged(PhTime time)
{
	if(!_settingStripTime) {
		if(qAbs(_sonyClock->time() - time) > 1000) {
			PHDEBUG << "error :" << _sonyClock->time() - time;
			// TODO handle frame difference error
			_settingSonyTime = true;
			_sonyClock->setTime(time);
			_settingSonyTime = false;
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
