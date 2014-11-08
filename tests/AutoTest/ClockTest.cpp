/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "PhTools/PhDebug.h"

#include "ClockTest.h"

ClockTest::ClockTest() : _time(0)
{

}

void ClockTest::onTimeChanged(PhTime time)
{
	_time = time;
	_timeChangedCalled = true;
}

void ClockTest::onRateChanged(PhRate rate)
{
	_rate = rate;
	_rateChangedCalled = true;
}

void ClockTest::initTestCase()
{
	connect(&_clock, &PhClock::timeChanged, this, &ClockTest::onTimeChanged);
	connect(&_clock, &PhClock::rateChanged, this, &ClockTest::onRateChanged);
}

void ClockTest::init()
{
	_clock.setTime(0);
	_clock.setRate(0);
	_timeChangedCalled = false;
	_rateChangedCalled = false;
}

void ClockTest::timeTest()
{
	QCOMPARE(_clock.time(), 0);

	// The time signal should be called if changing the time of a little value
	_clock.setTime(10);
	QVERIFY(_clock.time() == 10);
	QVERIFY(_timeChangedCalled);
	QVERIFY(_time == 10);

	// The time signal should not be called if changing to the same value
	_timeChangedCalled = false;
	_clock.setTime(10);

	QVERIFY(!_timeChangedCalled);
}

void ClockTest::frameTest()
{
	QCOMPARE(_clock.time(), 0);

	// The time and frame signal should be called if changing the frame value
	_clock.setFrame(10, PhTimeCodeType25);
	QVERIFY(_clock.time() == 9600);
	QVERIFY(_timeChangedCalled);
	QVERIFY(_time == 9600);
	QVERIFY(_clock.frame(PhTimeCodeType25) == 10);
}

void ClockTest::rateTest()
{
	QVERIFY(_clock.rate() == 0);

	// The rate signal should be called if changing the rate value
	_clock.setRate(10);
	QVERIFY(_clock.rate() == 10);
	QVERIFY(_rateChangedCalled);
	QVERIFY(_rate == 10);
}

void ClockTest::msTest()
{
	QVERIFY(_clock.milliSecond() == 0);

	_clock.setMillisecond(1000);

	QVERIFY(_clock.milliSecond() == 1000);
	QVERIFY(_clock.time() == 24000);
	QVERIFY(_clock.frame(PhTimeCodeType25) == 25);

	QVERIFY(_timeChangedCalled);
}

void ClockTest::tcTest()
{
	QCOMPARE(QString(_clock.timeCode(PhTimeCodeType25)), QString("00:00:00:00"));

	_clock.setTimeCode("00:00:01:00", PhTimeCodeType25);

	QCOMPARE(QString(_clock.timeCode(PhTimeCodeType25)), QString("00:00:01:00"));
	QVERIFY(_clock.time() == 24000);
	QVERIFY(_clock.frame(PhTimeCodeType25) == 25);

	QVERIFY(_timeChangedCalled);
}

void ClockTest::tickTest()
{
	QCOMPARE(_clock.time(), 0);

	// Ticking a clock with a null rate should not change the time
	_clock.elapse(24000); // ticking 1 second

	QVERIFY(!_timeChangedCalled);
	QCOMPARE(_clock.time(), 0);

	// Ticking a clock with a rate of 1 should move the time forward
	_clock.setRate(1);
	_clock.elapse(24000); // ticking 1 second
	QVERIFY(_timeChangedCalled);
	QCOMPARE(_clock.time(), 24000);

	// Ticking a clock with a rate of -2 should move the time backward
	_clock.setRate(-2);
	_clock.elapse(6000); // ticking 0.25 second
	QCOMPARE(_clock.time(), 12000);

}
