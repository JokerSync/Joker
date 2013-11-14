#include "PhTools/PhDebug.h"
#include "SonyVideoStripSynchronizer.h"

VideoStripSynchronizer::VideoStripSynchronizer()
	: _stripClock(NULL),
	  _videoClock(NULL),
	  _sonyClock(NULL),
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
}

void VideoStripSynchronizer::setSonyClock(PhClock *clock)
{
	_sonyClock = clock;
	connect(_sonyClock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onSonyFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_sonyClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onSonyRateChanged(PhRate)));
}

void VideoStripSynchronizer::onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(!_settingStripFrame)
	{
		_settingVideoFrame = true;
		_videoClock->setFrame(frame);
		_settingVideoFrame = false;
		if(_sonyClock)
		{
			// Apply precise correction.
			// We don't change sony clock because this would desynchronize the sony master.
			if(qAbs(frame - _sonyClock->frame()) > 1)
			{
				_settingStripFrame = true;
				_stripClock->setFrame(_sonyClock->frame());
				_settingStripFrame = false;
			}
		}
	}
}

void VideoStripSynchronizer::onStripRateChanged(PhRate rate)
{
	if(!_settingStripRate)
	{
		if(_sonyClock)
		{
			_settingSonyRate = true;
			_sonyClock->setRate(rate);
			_settingSonyRate = false;
		}
		_settingVideoRate = true;
		_videoClock->setRate(rate);
		_settingVideoRate = false;
	}
}

void VideoStripSynchronizer::onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
}

void VideoStripSynchronizer::onVideoRateChanged(PhRate rate)
{
	if(!_settingVideoRate)
	{
		_settingStripRate = true;
		_stripClock->setRate(rate);
		_settingStripRate = false;
	}
}

void VideoStripSynchronizer::onSonyFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(!_settingSonyFrame)
	{
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
		_settingStripRate = true;
		_stripClock->setRate(rate);
		_settingStripRate = false;
		_settingVideoRate = true;
		_videoClock->setRate(rate);
		_settingVideoFrame = false;
	}
}
