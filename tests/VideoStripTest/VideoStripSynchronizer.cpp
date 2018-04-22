#include "PhTools/PhDebug.h"
#include "VideoStripSynchronizer.h"

VideoStripSynchronizer::VideoStripSynchronizer() : _stripClock(NULL), _videoClock(NULL)
{
}

void VideoStripSynchronizer::setStripClock(PhClock *clock)
{
	_stripClock = clock;
	connect(_stripClock, &PhClock::timeChanged, this, &VideoStripSynchronizer::onStripTimeChanged);
	connect(_stripClock, &PhClock::rateChanged, this, &VideoStripSynchronizer::onStripRateChanged);
}

void VideoStripSynchronizer::setVideoClock(PhClock *clock)
{
	_videoClock = clock;
	connect(_videoClock, &PhClock::timeChanged, this, &VideoStripSynchronizer::onVideoTimeChanged);
	connect(_videoClock, &PhClock::rateChanged, this, &VideoStripSynchronizer::onVideoRateChanged);
}

void VideoStripSynchronizer::onStripTimeChanged(PhTime time)
{
	_videoClock->setTime(time);
}

void VideoStripSynchronizer::onStripRateChanged(PhRate)
{
}

void VideoStripSynchronizer::onVideoTimeChanged(PhTime time)
{
	if(_stripClock->time() != time)
		PHDEBUG << "error :" << _stripClock->time() << time;
	// TODO handle time difference error
}

void VideoStripSynchronizer::onVideoRateChanged(PhRate rate)
{
	_stripClock->setRate(rate);
}
