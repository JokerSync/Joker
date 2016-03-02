/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhSynchronizer.h"

PhSynchronizer::PhSynchronizer(PhSyncSettings *settings)
	: _settings(settings),
	_syncType(NoSync),
	_stripClock(NULL),
	_videoClock(NULL),
	_syncClock(NULL),
	_settingStripTime(false),
	_settingVideoTime(false),
	_settingSonyTime(false),
	_settingStripRate(false),
	_settingVideoRate(false),
	_settingSyncRate(false)
{
}

PhTime PhSynchronizer::time()
{
	if(_videoClock)
		return _videoClock->time();
	else if(_stripClock)
		return _stripClock->time();
	else if(_syncClock)
		return _syncClock->time();
	else
		return 0;
}

void PhSynchronizer::setTime(PhTime value)
{
	if(_videoClock) {
		_settingVideoTime = true;
		_videoClock->setTime(value);
		_settingVideoTime = false;
	}
	if(_stripClock)
		_stripClock->setTime(value);
	if(_syncClock)
		_syncClock->setTime(value);
}

PhRate PhSynchronizer::rate()
{
	if(_videoClock)
		return _videoClock->rate();
	else if(_stripClock)
		return _stripClock->rate();
	else if(_syncClock)
		return _syncClock->rate();
	else
		return 0;
}

void PhSynchronizer::setRate(PhRate value)
{
	if(_videoClock) {
		_settingVideoRate = true;
		_videoClock->setRate(value);
		_settingVideoRate = false;
	}
	if(_stripClock) {
		_settingStripRate = true;
		_stripClock->setRate(value);
		_settingStripRate = false;
	}
	if(_syncClock) {
		_settingSyncRate = true;
		_syncClock->setRate(value);
		_settingSyncRate = false;
	}
}

void PhSynchronizer::setStripClock(PhClock *clock)
{
	if(_stripClock) {
		disconnect(_stripClock, &PhClock::timeChanged, this, &PhSynchronizer::onStripTimeChanged);
		disconnect(_stripClock, &PhClock::rateChanged, this, &PhSynchronizer::onStripRateChanged);
	}
	_stripClock = clock;
	if(clock) {
		connect(_stripClock, &PhClock::timeChanged, this, &PhSynchronizer::onStripTimeChanged);
		connect(_stripClock, &PhClock::rateChanged, this, &PhSynchronizer::onStripRateChanged);
	}
}

void PhSynchronizer::setVideoClock(PhClock *clock)
{
	if(_videoClock) {
		disconnect(_videoClock, &PhClock::timeChanged, this, &PhSynchronizer::onVideoTimeChanged);
		disconnect(_videoClock, &PhClock::rateChanged, this, &PhSynchronizer::onVideoRateChanged);
	}
	_videoClock = clock;
	if(clock) {
		connect(_videoClock, &PhClock::timeChanged, this, &PhSynchronizer::onVideoTimeChanged);
		connect(_videoClock, &PhClock::rateChanged, this, &PhSynchronizer::onVideoRateChanged);
	}
}

void PhSynchronizer::setSyncClock(PhClock *clock, SyncType type)
{
	if(_syncClock) {
		disconnect(_syncClock, &PhClock::timeChanged, this, &PhSynchronizer::onSyncTimeChanged);
		disconnect(_syncClock, &PhClock::rateChanged, this, &PhSynchronizer::onSyncRateChanged);
	}
	_syncClock = clock;
	_syncType = type;
	if(_syncClock) {
		connect(_syncClock, &PhClock::timeChanged, this, &PhSynchronizer::onSyncTimeChanged);
		connect(_syncClock, &PhClock::rateChanged, this, &PhSynchronizer::onSyncRateChanged);
	}
}

void PhSynchronizer::onStripTimeChanged(PhTime time)
{
	if(!_settingStripTime) {
		_settingStripTime = true;
		if(_settings->syncLooping() && (this->rate() != 0) && (time > _settings->syncLoopTimeOut())) {
			this->setTime(_settings->syncLoopTimeIn());
		}
		else {
			if(_syncClock) {
				// Apply precise correction.
				// We use a 2-frame error margin, which is consistent with LTC needs
				// We don't change sony clock because this would desynchronize the sony master.
#warning /// @todo Make the error a settings
				if(qAbs(time - _syncClock->time()) > 2*PhTimeCode::timePerFrame(PhTimeCodeType24)) {
					PHDEBUG << "correct :" << _stripClock->time() << _syncClock->time();
					_stripClock->setTime(_syncClock->time());
				}
			}

			if((_syncType != Sony) && _videoClock) {
				_settingVideoTime = true;
				_videoClock->setTime(time);
				_settingVideoTime = false;
			}
		}
		_settingStripTime = false;
	}
}

void PhSynchronizer::onStripRateChanged(PhRate rate)
{
	if(!_settingStripRate) {
		PHDEBUG << rate;
		if(_syncClock) {
			_settingSyncRate = true;
			_syncClock->setRate(rate);
			_settingSyncRate = false;
		}
		if(_videoClock) {
			_settingVideoRate = true;
			_videoClock->setRate(rate);
			_settingVideoRate = false;
		}
	}
}

void PhSynchronizer::onVideoTimeChanged(PhTime)
{
	//	if(!_settingVideoTime)
	//		PHDEBUG << time;
}

void PhSynchronizer::onVideoRateChanged(PhRate)
{
}

void PhSynchronizer::onSyncTimeChanged(PhTime time)
{
	if(!_settingSonyTime) {
		if(_settings->syncLooping() && (time > _settings->syncLoopTimeOut())) {
			this->setTime(_settings->syncLoopTimeIn());
		}
		else {
			if((_syncType != LTC) && _videoClock) {
				_settingVideoTime = true;
				_videoClock->setTime(time);
				_settingVideoTime = false;
			}
			// We apply correction here only when there is a significant change of sony time.
			// Precise correction occurs in onStripTimeChanged() that is called after
			// on SonyTimeChanged (see VideoStripView::paint()).
			if(_stripClock) {
				PhTime error = qAbs(time - _stripClock->time());
				if(((error > 10 * PhTimeCode::timePerFrame(PhTimeCodeType24))
				    || ((_stripClock->rate() == 0) && (error > 0)))) {
					PHDEBUG << "correct error:" << time << _stripClock->time();
					_settingStripTime = true;
					_stripClock->setTime(time);
					_settingStripTime = false;
				}
			}
		}
	}
}

void PhSynchronizer::onSyncRateChanged(PhRate rate)
{
	if(!_settingSyncRate) {
		PHDEBUG << rate;
		if(_stripClock) {
			_settingStripRate = true;
			_stripClock->setRate(rate);
			_settingStripRate = false;
		}
		if(_videoClock) {
			_settingVideoRate = true;
			_videoClock->setRate(rate);
			_settingVideoTime = false;
		}
	}
}
