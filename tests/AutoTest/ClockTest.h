/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef CLOCKTEST_H
#define CLOCKTEST_H

#include <QObject>

#include "PhSync/PhClock.h"

class ClockTest : public QObject
{
	Q_OBJECT
public:
	ClockTest();

protected slots:
	void onTimeChanged(PhTime time);
	void onRateChanged(PhRate rate);

private slots:
	void initTestCase();
	void init();
	void timeTest();
	void frameTest();
	void rateTest();
	void msTest();
	void tcTest();
	void tickTest();

private:
	PhClock _clock;
	PhTime _time;
	PhRate _rate;
	bool _timeChangedCalled;
	bool _rateChangedCalled;
};

#endif // CLOCKTEST_H

