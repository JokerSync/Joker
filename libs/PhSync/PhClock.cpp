/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhClock.h"

PhClock::PhClock() :
	QObject(NULL), _time(0), _rate(0.0)
{
	qRegisterMetaType<PhTime>("PhTime");
	qRegisterMetaType<PhFrame>("PhFrame");
	qRegisterMetaType<PhRate>("PhRate");
	qRegisterMetaType<PhTimeCodeType>("PhTimeCodeType");
}

void PhClock::setTime(qint64 time)
{
	if (_time != time) {
		_time = time;
		emit timeChanged(time);
	}
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

void PhClock::setFrame(PhFrame frame, PhTimeCodeType tcType)
{
	this->setTime(frame * PhTimeCode::timePerFrame(tcType));
}

void PhClock::setFrame25(PhFrame frame)
{
	this->setFrame(frame, PhTimeCodeType25);
}

PhFrame PhClock::frame(PhTimeCodeType tcType) const
{
	return PhTimeCode::frameFromTime(_time, tcType);
}

void PhClock::setTimeCode(QString tc, PhTimeCodeType tcType)
{
	setTime(PhTimeCode::timeFromString(tc, tcType));
}

QString PhClock::timeCode(PhTimeCodeType tcType)
{
	return PhTimeCode::stringFromTime(_time, tcType);
}

void PhClock::elapse(PhTime elapsedTime)
{
	this->setTime(_time + elapsedTime * _rate);
}

