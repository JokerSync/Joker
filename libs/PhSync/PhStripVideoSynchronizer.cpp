#include "PhStripVideoSynchronizer.h"
#include "PhTools/PhDebug.h"

PhStripVideoSynchronizer::PhStripVideoSynchronizer() : _stripClock(NULL), _videoClock(NULL)
{
}

void PhStripVideoSynchronizer::setStripClock(PhClock *clock)
{
	_stripClock = clock;
	connect(_stripClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onStripFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void PhStripVideoSynchronizer::setVideoClock(PhClock *clock)
{
	_videoClock = clock;
	connect(_videoClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onVideoFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_videoClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onVideoRateChanged(PhRate)));
}

void PhStripVideoSynchronizer::onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	_videoClock->setFrame(frame);
}

void PhStripVideoSynchronizer::onStripRateChanged(PhRate rate)
{
	//_videoClock->setRate(rate);
}

void PhStripVideoSynchronizer::onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(_stripClock->frame() != frame)
		PHDEBUG << "error :" << _stripClock->frame() << frame;
#warning TODO handle frame difference error
}

void PhStripVideoSynchronizer::onVideoRateChanged(PhRate rate)
{
	_stripClock->setRate(rate);
}
