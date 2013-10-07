#include "PhTools/PhDebug.h"
#include "SonyVideoStripSynchronizer.h"

VideoStripSynchronizer::VideoStripSynchronizer()
	: _stripClock(NULL),
	  _videoClock(NULL),
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
		_settingSonyFrame = true;
		_sonyClock->setFrame(frame);
		_settingSonyFrame = false;
	}
}

void VideoStripSynchronizer::onStripRateChanged(PhRate rate)
{
	if(!_settingStripRate)
	{
		_settingSonyRate = true;
		_sonyClock->setRate(rate);
		_settingSonyRate = false;
	}
}

void VideoStripSynchronizer::onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
#warning TODO handle frame difference error
	if(!_settingVideoFrame)
	{
		if(_stripClock->frame() != frame)
			PHDEBUG << "error :" << _stripClock->frame() << frame;
	}
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
		_settingStripFrame = true;
		_stripClock->setFrame(frame);
		_settingStripFrame = false;
	}
}

void VideoStripSynchronizer::onSonyRateChanged(PhRate rate)
{
	if(!_settingSonyRate)
	{
		_settingStripRate = true;
		_stripClock->setRate(rate);
		_settingStripRate = false;
	}
}
