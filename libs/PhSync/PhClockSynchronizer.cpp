#include "PhClockSynchronizer.h"
#include "PhTools/PhDebug.h"

PhClockSynchronizer::PhClockSynchronizer()
{
}

void PhClockSynchronizer::setPrimaryClock(PhClock *clock)
{
	_clock1 = clock;
}

void PhClockSynchronizer::setSecondaryClock(PhClock *clock)
{
	_clock2 = clock;
	connect(_clock2, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_clock2, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
}

void PhClockSynchronizer::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(_clock1->frame() != frame)
		PHDEBUG << "error :" << _clock1->frame() << frame;
#warning TODO handle frame difference error
}

void PhClockSynchronizer::onRateChanged(PhRate rate)
{
	_clock1->setRate(rate);
}
