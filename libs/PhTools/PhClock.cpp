/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhClock.h"

PhClock::PhClock(PhTimeCodeType tcType) :
	QObject(NULL), _tcType(tcType), _time(0), _rate(0.0)
{
	qRegisterMetaType<PhTime>("PhTime");
	qRegisterMetaType<PhFrame>("PhFrame");
	qRegisterMetaType<PhRate>("PhRate");
	qRegisterMetaType<PhTimeCodeType>("PhTimeCodeType");
}

void PhClock::setTimeCodeType(PhTimeCodeType tcType)
{
	PhFrame lastFrame = frame();
	if(_tcType != tcType) {
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

void PhClock::setRate(PhRate rate)
{
	if (_rate != rate) {
		_rate = rate;
		emit rateChanged(rate);
	}
}

void PhClock::setMillisecond(PhTime ms)
{
	this->setTime(ms * 24);
}

PhTime PhClock::milliSecond()
{
	return _time / 24;
}

void PhClock::setFrame(PhFrame frame)
{
	this->setTime(frame * PhTimeCode::timePerFrame(_tcType));
}

PhFrame PhClock::frame() const
{
	return _time / PhTimeCode::timePerFrame(_tcType);
}

void PhClock::setTimeCode(QString tc)
{
	setFrame(PhTimeCode::frameFromString(tc, _tcType));
}

QString PhClock::timeCode()
{
	return PhTimeCode::stringFromFrame(frame(), _tcType);
}

void PhClock::tick(PhTimeScale frequence)
{
	int elapsed = 24000 / frequence;
	this->setTime(static_cast<int>(_time + elapsed * _rate));
}

