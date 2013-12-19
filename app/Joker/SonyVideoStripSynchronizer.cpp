#include "PhTools/PhDebug.h"
#include "SonyVideoStripSynchronizer.h"

VideoStripSynchronizer::VideoStripSynchronizer()
	: _stripClock(NULL),
	  _videoClock(NULL),
	  _syncClock(NULL),
	  _settingStripFrame(false),
	  _settingVideoFrame(false),
	  _settingSonyFrame(false),
	  _settingStripRate(false),
	  _settingVideoRate(false),
	  _settingSonyRate(false)
{
}

void VideoStripSynchronizer::setStripClock(PhClock *clock)
{
	_stripClock = clock;
	connect(_stripClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onStripFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onStripRateChanged(PhRate)));
}

void VideoStripSynchronizer::setVideoClock(PhClock *clock)
{
	_videoClock = clock;
	connect(_videoClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onVideoFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_videoClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onVideoRateChanged(PhRate)));
	connect(_videoClock, SIGNAL(tcTypeChanged(PhTimeCodeType)), this, SLOT(onVideoTCTypeChanged(PhTimeCodeType)));
}

void VideoStripSynchronizer::setSyncClock(PhClock *clock)
{
	_syncClock = clock;
	connect(_syncClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onSonyFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_syncClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onSonyRateChanged(PhRate)));
}

void VideoStripSynchronizer::onStripFrameChanged(PhFrame frame, PhTimeCodeType)
{
	if(!_settingStripFrame)
	{
		PHDBG(2) << frame;
		if(_syncClock)
		{
			// Apply precise correction.
			// We don't change sony clock because this would desynchronize the sony master.
			if(qAbs(frame - _syncClock->frame()) > 1)
			{
				PHDEBUG << "correct :" << _stripClock->frame() << _syncClock->frame();
				_settingStripFrame = true;
				_stripClock->setFrame(_syncClock->frame());
				_settingStripFrame = false;
			}
		}
		else
		{
			_settingVideoFrame = true;
			_videoClock->setFrame(frame);
			_settingVideoFrame = false;
		}
	}
}

void VideoStripSynchronizer::onStripRateChanged(PhRate rate)
{
	if(!_settingStripRate)
	{
		PHDEBUG << rate;
		if(_syncClock)
		{
			_settingSonyRate = true;
			_syncClock->setRate(rate);
			_settingSonyRate = false;
		}
		_settingVideoRate = true;
		_videoClock->setRate(rate);
		_settingVideoRate = false;
	}
}

void VideoStripSynchronizer::onVideoFrameChanged(PhFrame, PhTimeCodeType)
{
//	if(!_settingVideoFrame)
//		PHDEBUG << frame;
}

void VideoStripSynchronizer::onVideoRateChanged(PhRate)
{
}

void VideoStripSynchronizer::onVideoTCTypeChanged(PhTimeCodeType tcType)
{
	PHDEBUG << tcType;
	_stripClock->setTimeCodeType(tcType);
	if(_syncClock)
		_syncClock->setTimeCodeType(tcType);
}

void VideoStripSynchronizer::onSonyFrameChanged(PhFrame frame, PhTimeCodeType)
{
	if(!_settingSonyFrame)
	{
		PHDBG(3) << frame;
		_settingVideoFrame = true;
		_videoClock->setFrame(frame);
		_settingVideoFrame = false;
		// We apply correction here only when there is a significant change of sony frame.
		// Precise correction occurs in onStripFrameChanged() that is called after
		// on SonyFrameChanged (see VideoStripView::paint()).
		if(qAbs(frame - _stripClock->frame()) > 10)
		{
			PHDEBUG << "correct error:" << frame << _stripClock->frame();
			_settingStripFrame = true;
			_stripClock->setFrame(frame);
			_settingStripFrame = false;
		}
	}
}

void VideoStripSynchronizer::onSonyRateChanged(PhRate rate)
{
	if(!_settingSonyRate)
	{
		PHDEBUG << rate;
		_settingStripRate = true;
		_stripClock->setRate(rate);
		_settingStripRate = false;
		_settingVideoRate = true;
		_videoClock->setRate(rate);
		_settingVideoFrame = false;
	}
}
