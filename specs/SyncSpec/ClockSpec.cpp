/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"
#include "PhSync/PhClock.h"

using namespace igloo;

Describe(a_clock_test) {
	PhClock _clock;
	PhTime _time;
	PhRate _rate;
	bool _timeChanged;
	bool _rateChanged;

	void SetUp() {
		_clock.setTime(0);
		_clock.setRate(0);
		_time = 0;
		_rate = 0;
		_timeChanged = false;
		_rateChanged = false;
		QObject::connect(&_clock, &PhClock::timeChanged, [&](PhTime time) {
			_time = time;
			_timeChanged = true;
		});
		QObject::connect(&_clock, &PhClock::rateChanged, [&](PhRate rate) {
			_rate = rate;
			_rateChanged = true;
		});
	}

	It(doesnt_call_time_changed_if_no_modification) {
		Assert::That(_clock.time(), Equals(0));
		Assert::That(!_timeChanged);
		Assert::That(_time, Equals(0));
		Assert::That(_clock.time(), Equals(0));
	}

	It(call_time_changed_upon_time_modification) {
		_clock.setTime(1);
		Assert::That(_timeChanged);
		Assert::That(_time, Equals(1));
		Assert::That(_clock.time(), Equals(1));
	}

	It(call_time_changed_upon_frame_modification) {
		Assert::That(_clock.time(), Equals(0));
		Assert::That(!_timeChanged);
		_clock.setFrame(10, PhTimeCodeType25);
		Assert::That(_timeChanged);
		Assert::That(_time, Equals(9600));
		Assert::That(_clock.time(), Equals(9600));
	}

	It(call_time_changed_upon_ms_modification) {
		Assert::That(_clock.time(), Equals(0));
		Assert::That(!_timeChanged);
		_clock.setFrame(10, PhTimeCodeType25);
		Assert::That(_timeChanged);
		Assert::That(_time, Equals(9600));
		Assert::That(_clock.time(), Equals(9600));
	}

	It(doesnt_call_rate_changed_if_no_modification) {
		Assert::That(_clock.rate(), Equals(0));
		Assert::That(!_rateChanged);
		Assert::That(_rate, Equals(0));
		Assert::That(_clock.rate(), Equals(0));
	}

	It(call_rate_changed_upon_rate_modification) {
		_clock.setRate(1);
		Assert::That(_rateChanged);
		Assert::That(_rate, Equals(1));
		Assert::That(_clock.rate(), Equals(1));
	}
};

