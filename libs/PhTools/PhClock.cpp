#include "PhClock.h"

PhClock::PhClock(QObject *parent) :
	QObject(parent), _time(0), _timeScale(600), _rate(0.0f)
{
}

/****************************Slots****************************/

void PhClock::tick(int frequence)
{
	int elapsed = _timeScale / frequence;
	this->setTime(static_cast<int>(_time + elapsed * _rate));
}

/****************************Getters****************************/

void PhClock::setTime(qint64 time)
{
	if (_time != time) {
		_time = time;
		emit timeChanged(time);
	}
}

void PhClock::setTimeScale(int timeScale)
{
	_timeScale = timeScale;
}

void PhClock::setRate(float rate)
{
	if (_rate != rate) {
		_rate = rate;
		emit rateChanged(rate);
	}
}

int PhClock::frame(PhTimeCodeType tcType) const
{
	int fps = PhTimeCode::getFps(tcType);
	return _time * fps / _timeScale;
}

void PhClock::setFrame(int frame, PhTimeCodeType tcType)
{
	int fps = PhTimeCode::getFps(tcType);
	this->setTime(frame * _timeScale / fps);
}

