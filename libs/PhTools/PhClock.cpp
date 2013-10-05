#include "PhClock.h"

PhClock::PhClock(PhTimeCodeType tcType, QObject *parent) :
	QObject(parent), _tcType(tcType), _time(0), _timeScale(600), _rate(0.0)
{
}

///****************************Properties****************************/

void PhClock::setTimeCodeType(PhTimeCodeType tcType)
{
	PhFrame lastFrame = frame();
	if(_tcType != tcType)
	{
		_tcType = tcType;
		emit tcTypeChanged(tcType);
	}
	PhFrame newFrame = frame();
	if(lastFrame != newFrame)
		emit frameChanged(newFrame, _tcType);
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
		emit frameChanged(newFrame, _tcType);
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

void PhClock::setMillisecond(PhTime ms)
{
	this->setTime(ms * _timeScale / 1000);
}

PhTime PhClock::milliSecond()
{
	return _time * 1000 / _timeScale;
}

void PhClock::setFrame(PhFrame frame)
{
	int fps = PhTimeCode::getFps(_tcType);
	this->setTime(frame * _timeScale / fps);
}

PhFrame PhClock::frame() const
{
	int fps = PhTimeCode::getFps(_tcType);
	return _time * fps / _timeScale;
}

void PhClock::setTimeCode(QString tc)
{
	setFrame(PhTimeCode::frameFromString(tc, _tcType));
}

QString PhClock::timeCode()
{
	return PhTimeCode::stringFromFrame(frame(), _tcType);
}

///****************************Slots****************************/

void PhClock::tick(PhTimeScale frequence)
{
	int elapsed = _timeScale / frequence;
	this->setTime(static_cast<int>(_time + elapsed * _rate));
}


