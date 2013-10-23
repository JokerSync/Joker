#include "SonyStripSynchronizer.h"
#include "PhTools/PhDebug.h"

SonyStripSynchronizer::SonyStripSynchronizer()
{
}

void SonyStripSynchronizer::setSonyClock(PhClock *clock)
{
	_sonyClock = clock;
	connect(_sonyClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onSonyFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_sonyClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onSonyRateChanged(PhRate)));
}

void SonyStripSynchronizer::setStripClock(PhClock *clock)
{
	_stripClock = clock;
	connect(_stripClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onStripFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void SonyStripSynchronizer::onSonyFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	_stripClock->setFrame(frame);
}

void SonyStripSynchronizer::onSonyRateChanged(PhRate rate)
{
	_stripClock->setRate(rate);
}

void SonyStripSynchronizer::onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(_sonyClock->frame() != frame)
		PHDEBUG << "error :" << _sonyClock->frame() << frame;
#warning TODO handle frame difference error
}

void SonyStripSynchronizer::onStripRateChanged(PhRate rate)
{
	_sonyClock->setRate(rate);
}
