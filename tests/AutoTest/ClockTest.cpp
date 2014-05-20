/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "ClockTest.h"


ClockTest::ClockTest() : _clock(PhTimeCodeType25),
	_time(0),
	_frame(0),
	_tcType(PhTimeCodeType25)
{

}

void ClockTest::onTimeChanged(PhTime time)
{
	_time = time;
	_timeChangedCalled = true;
}

void ClockTest::onFrameChanged(PhFrame frame, PhTimeCodeType)
{
	_frame = frame;
	_frameChangedCalled = true;
}

void ClockTest::onTimeCodeTypeChanged(PhTimeCodeType tcType)
{
	_tcType = tcType;
	_tcTypeChangedCalled = true;
}

void ClockTest::onRateChanged(PhRate rate)
{
	_rate = rate;
	_rateChangedCalled = true;
}

void ClockTest::initTestCase()
{
	connect(&_clock, &PhClock::timeChanged, this, &ClockTest::onTimeChanged);
	connect(&_clock, &PhClock::frameChanged, this, &ClockTest::onFrameChanged);
	connect(&_clock, &PhClock::tcTypeChanged, this, &ClockTest::onTimeCodeTypeChanged);
	connect(&_clock, &PhClock::rateChanged, this, &ClockTest::onRateChanged);
}

void ClockTest::init()
{
	_clock.setTime(0);
	_clock.setTimeCodeType(PhTimeCodeType25);
	_clock.setRate(0);
	_frameChangedCalled = false;
	_tcTypeChangedCalled = false;
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

	// But not the frame signal
	QVERIFY(!_frameChangedCalled);

	// The time signal should not be called if changing to the same value
	_timeChangedCalled = false;
	_clock.setTime(10);

	QVERIFY(!_timeChangedCalled);
	QVERIFY(!_frameChangedCalled);

	// The time and frame signal should be called if changing the time of a bigger value
	_clock.setTime(960);
	QVERIFY(_clock.time() == 960);
	QVERIFY(_timeChangedCalled);
	QVERIFY(_time == 960);
	QVERIFY(_clock.frame() == 1);
	QVERIFY(_frameChangedCalled);
	QVERIFY(_frame == 1);
}

void ClockTest::frameTest()
{
	QCOMPARE(_clock.time(), 0);

	// The time and frame signal should be called if changing the frame value
	_clock.setFrame(10);
	QVERIFY(_clock.time() == 9600);
	QVERIFY(_timeChangedCalled);
	QVERIFY(_time == 9600);
	QVERIFY(_clock.frame() == 10);
	QVERIFY(_frameChangedCalled);
	QVERIFY(_frame == 10);
}

void ClockTest::tcTypeTest()
{
	QCOMPARE(_clock.timeCodeType(), PhTimeCodeType25);
	QVERIFY(_clock.frame() == 0);

	// The tc type signal shall not be called if settings the same tc type
	_clock.setTimeCodeType(PhTimeCodeType25);

	QVERIFY(!_tcTypeChangedCalled);
	QVERIFY(!_frameChangedCalled);

	// The tc type signal shall be called if settings a different tc type
	_clock.setTimeCodeType(PhTimeCodeType24);

	QCOMPARE(_clock.timeCodeType(), PhTimeCodeType24);
	QVERIFY(_tcTypeChangedCalled);
	QCOMPARE(_tcType, PhTimeCodeType24);

	// But not the frame signal
	QVERIFY(!_frameChangedCalled);

	// Test at another frame value
	_clock.setFrame(23);
	_tcTypeChangedCalled = false;
	_frameChangedCalled = false;

	// The frame signal shall be call
	_clock.setTimeCodeType(PhTimeCodeType2997);

	QCOMPARE(_clock.timeCodeType(), PhTimeCodeType2997);
	QVERIFY(_tcTypeChangedCalled);
	QCOMPARE(_tcType, PhTimeCodeType2997);
	QVERIFY(_frameChangedCalled);
	PHDEBUG << _frame;
	QVERIFY(_frame == 28);
	QVERIFY(_clock.frame() == 28);
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
	QVERIFY(_clock.frame() == 25);

	QVERIFY(_timeChangedCalled);
	QVERIFY(_frameChangedCalled);
}

void ClockTest::tcTest()
{
	QCOMPARE(QString(_clock.timeCode()), QString("00:00:00:00"));

	_clock.setTimeCode("00:00:01:00");

	QCOMPARE(QString(_clock.timeCode()), QString("00:00:01:00"));
	QVERIFY(_clock.time() == 24000);
	QVERIFY(_clock.frame() == 25);

	QVERIFY(_timeChangedCalled);
	QVERIFY(_frameChangedCalled);
}

void ClockTest::tickTest()
{
	QCOMPARE(_clock.time(), 0);

	// Ticking a clock with a null rate should not change the time
	_clock.tick(1); // ticking 1Hz = 1 second

	QVERIFY(!_timeChangedCalled);
	QCOMPARE(_clock.time(), 0);

	// Ticking a clock with a rate of 1 should move the time forward
	_clock.setRate(1);
	_clock.tick(1); // ticking 1Hz = 1 second
	QVERIFY(_timeChangedCalled);
	QCOMPARE(_clock.time(), 24000);

	// Ticking a clock with a rate of -2 should move the time backward
	_clock.setRate(-2);
	_clock.tick(4); // ticking 4Hz = 0.25 second
	QCOMPARE(_clock.time(), 12000);

}
