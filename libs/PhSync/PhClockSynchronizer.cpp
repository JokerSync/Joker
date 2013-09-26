#include "PhClockSynchronizer.h"
#include "PhTools/PhDebug.h"

PhClockSynchronizer::PhClockSynchronizer()
{
}

void PhClockSynchronizer::setPrimaryClock(PhClock *clock)
{
	_clock1 = clock;
	connect(_clock1, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onStripFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_clock1, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void PhClockSynchronizer::setSecondaryClock(PhClock *clock)
{
	_clock2 = clock;
	connect(_clock2, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onVideoFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_clock2, SIGNAL(rateChanged(PhRate)), this, SLOT(onVideoRateChanged(PhRate)));
}

void PhClockSynchronizer::onFrameChanged1(PhFrame frame, PhTimeCodeType tcType)
{
	_clock2->setFrame(frame);
}

void PhClockSynchronizer::onRateChanged1(PhRate rate)
{
	_clock2->setRate(rate);
}

void PhClockSynchronizer::onFrameChanged2(PhFrame frame, PhTimeCodeType tcType)
{
	if(_clock1->frame() != frame)
		PHDEBUG << "error :" << _clock1->frame() << frame;
#warning TODO handle frame difference error
}

void PhClockSynchronizer::onRateChanged2(PhRate rate)
{
	_clock1->setRate(rate);
}
