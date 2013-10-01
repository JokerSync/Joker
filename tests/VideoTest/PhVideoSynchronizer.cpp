#include "PhVideoSynchronizer.h"

PhVideoSynchronizer::PhVideoSynchronizer() : _internalClock(NULL), _videoClock(NULL)
{
}


void PhVideoSynchronizer::setInternalClock(PhClock *clock)
{
    _internalClock = clock;
    connect(_internalClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onInternalFrameChanged(PhFrame,PhTimeCodeType)));
    connect(_internalClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onInternalRateChanged(PhRate)));
}


void PhVideoSynchronizer::setVideoClock(PhClock *clock)
{
    _videoClock = clock;
    connect(_videoClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onVideoFrameChanged(PhFrame,PhTimeCodeType)));
    connect(_videoClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onVideoRateChanged(PhRate)));
}

void PhVideoSynchronizer::onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
    if(_internalClock->frame() != frame)
        PHDEBUG << "error :" << _internalClock->frame() << frame;
#warning TODO handle frame difference error
}

void PhVideoSynchronizer::onVideoRateChanged(PhRate rate)
{
    _internalClock->setRate(rate);
}

void PhVideoSynchronizer::onInternalFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
    _videoClock->setFrame(frame);
}

void PhVideoSynchronizer::onInternalRateChanged(PhRate rate)
{
    //_videoClock->setRate(rate);
}
