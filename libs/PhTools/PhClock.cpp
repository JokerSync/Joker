#include "PhClock.h"

PhClock::PhClock(QObject *parent) :
	QObject(parent), _tcType(PhTimeCodeType25), _time(0), _timeScale(600), _rate(0.0)
{
}

/****************************Properties****************************/

void PhClock::setTCType(PhTimeCodeType tcType)
{
	_tcType = tcType;
}

void PhClock::setTime(qint64 time)
{
	PhFrame lastFrame = frame();
	if (_time != time) {
		_time = time;
		emit timeChanged(time);
	}
	PhFrame newFrame = frame();
	if(lastFrame != newFrame)
		emit frameChanged();
}

void PhClock::setTimeScale(PhTimeScale timeScale)
{
	_timeScale = timeScale;
}

void PhClock::setRate(PhRate rate)
{
	if (_rate != rate) {
		_rate = rate;
		emit rateChanged(rate);
	}
}

int PhClock::frame() const
{
	int fps = PhTimeCode::getFps(_tcType);
	return _time * fps / _timeScale;
}

void PhClock::setFrame(PhFrame frame)
{
	int fps = PhTimeCode::getFps(_tcType);
	this->setTime(frame * _timeScale / fps);
}

/****************************Slots****************************/

void PhClock::tick(PhTimeScale frequence)
{
	int elapsed = _timeScale / frequence;
	this->setTime(static_cast<int>(_time + elapsed * _rate));
}


