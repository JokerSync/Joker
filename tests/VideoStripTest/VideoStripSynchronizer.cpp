#include "PhTools/PhDebug.h"
#include "VideoStripSynchronizer.h"

SonyVideoStripSynchronizer::SonyVideoStripSynchronizer() : _stripClock(NULL), _videoClock(NULL)
{
}

void SonyVideoStripSynchronizer::setStripClock(PhClock *clock)
{
	_stripClock = clock;
	connect(_stripClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onStripFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void SonyVideoStripSynchronizer::setVideoClock(PhClock *clock)
{
	_videoClock = clock;
	connect(_videoClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onVideoFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_videoClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onVideoRateChanged(PhRate)));
}

void SonyVideoStripSynchronizer::onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	_videoClock->setFrame(frame);
}

void SonyVideoStripSynchronizer::onStripRateChanged(PhRate rate)
{
	//_videoClock->setRate(rate);
}

void SonyVideoStripSynchronizer::onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(_stripClock->frame() != frame)
		PHDEBUG << "error :" << _stripClock->frame() << frame;
#warning TODO handle frame difference error
}

void SonyVideoStripSynchronizer::onVideoRateChanged(PhRate rate)
{
	_stripClock->setRate(rate);
}
