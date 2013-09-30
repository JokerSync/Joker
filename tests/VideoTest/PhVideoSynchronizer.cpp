#include "PhVideoSynchronizer.h"
#include "PhTools/PhDebug.h"

PhVideoSynchronizer::PhVideoSynchronizer() : _stripClock(NULL), _videoClock(NULL)
{
}

void PhVideoSynchronizer::setStripClock(PhClock *clock)
{
    _stripClock = clock;
    connect(_stripClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onStripFrameChanged(PhFrame,PhTimeCodeType)));
    connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void PhVideoSynchronizer::setVideoClock(PhClock *clock)
{
    _videoClock = clock;
    connect(_videoClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onVideoFrameChanged(PhFrame,PhTimeCodeType)));
    connect(_videoClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onVideoRateChanged(PhRate)));
}

void PhVideoSynchronizer::onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
    _videoClock->setFrame(frame);
}

void PhVideoSynchronizer::onStripRateChanged(PhRate rate)
{
    //_videoClock->setRate(rate);
}

void PhVideoSynchronizer::onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
    if(_stripClock->frame() != frame)
        PHDEBUG << "error :" << _stripClock->frame() << frame;
#warning TODO handle frame difference error
}

void PhVideoSynchronizer::onVideoRateChanged(PhRate rate)
{
    _stripClock->setRate(rate);
}
