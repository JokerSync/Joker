/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef CLOCKTEST_H
#define CLOCKTEST_H

#include <QObject>

#include "PhTools/PhClock.h"

class ClockTest : public QObject
{
	Q_OBJECT
public:
	ClockTest();

protected slots:
	void onTimeChanged(PhTime time);
	void onFrameChanged(PhFrame frame, PhTimeCodeType);
	void onTimeCodeTypeChanged(PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);

private slots:
	void initTestCase();
	void init();
	void timeTest();
	void frameTest();
	void tcTypeTest();
	void rateTest();
	void msTest();
	void tcTest();
	void tickTest();

private:
	PhClock _clock;
	PhTime _time;
	PhFrame _frame;
	PhTimeCodeType _tcType;
	PhRate _rate;
	bool _timeChangedCalled;
	bool _frameChangedCalled;
	bool _tcTypeChangedCalled;
	bool _rateChangedCalled;
};

#endif // CLOCKTEST_H

