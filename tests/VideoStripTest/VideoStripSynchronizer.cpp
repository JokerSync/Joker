#include "PhTools/PhDebug.h"
#include "VideoStripSynchronizer.h"

VideoStripSynchronizer::VideoStripSynchronizer() : _stripClock(NULL), _videoClock(NULL)
{
}

void VideoStripSynchronizer::setStripClock(PhClock *clock)
{
	_stripClock = clock;
	connect(_stripClock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onStripFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void VideoStripSynchronizer::setVideoClock(PhClock *clock)
{
	_videoClock = clock;
	connect(_videoClock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onVideoFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_videoClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onVideoRateChanged(PhRate)));
}

void VideoStripSynchronizer::onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	_videoClock->setFrame(frame);
}

void VideoStripSynchronizer::onStripRateChanged(PhRate rate)
{
	//_videoClock->setRate(rate);
}

void VideoStripSynchronizer::onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(_stripClock->frame() != frame)
		PHDEBUG << "error :" << _stripClock->frame() << frame;
#warning TODO handle frame difference error
}

void VideoStripSynchronizer::onVideoRateChanged(PhRate rate)
{
	_stripClock->setRate(rate);
}
