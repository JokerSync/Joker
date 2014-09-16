/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"
#include "PhSync/PhClock.h"

using namespace igloo;

Describe(clock_test) {
	PhClock clock;
	PhTime time;
	PhRate rate;
	bool timeChanged;
	bool rateChanged;

	void SetUp() {
		clock.setTime(0);
		clock.setRate(0);
		time = 0;
		rate = 0;
		timeChanged = false;
		rateChanged = false;
		QObject::connect(&clock, &PhClock::timeChanged, [&](PhTime t) {
			time = t;
			timeChanged = true;
		});
		QObject::connect(&clock, &PhClock::rateChanged, [&](PhRate r) {
			rate = r;
			rateChanged = true;
		});
	}

	It(doesnt_call_time_changed_if_no_modification) {
		Assert::That(clock.time(), Equals(0));
		Assert::That(!timeChanged);
		Assert::That(time, Equals(0));
		Assert::That(clock.time(), Equals(0));
	}

	It(call_time_changed_upon_time_modification) {
		clock.setTime(1);
		Assert::That(timeChanged);
		Assert::That(time, Equals(1));
		Assert::That(clock.time(), Equals(1));
	}

	It(call_time_changed_upon_frame_modification) {
		Assert::That(clock.time(), Equals(0));
		Assert::That(!timeChanged);
		clock.setFrame(10, PhTimeCodeType25);
		Assert::That(timeChanged);
		Assert::That(time, Equals(9600));
		Assert::That(clock.time(), Equals(9600));
	}

	It(call_time_changed_upon_millisecond_modification) {
		Assert::That(clock.time(), Equals(0));
		Assert::That(!timeChanged);
		clock.setMillisecond(1000);
		Assert::That(timeChanged);
		Assert::That(time, Equals(24000));
		Assert::That(clock.time(), Equals(24000));
	}

	It(call_time_changed_upon_timecode_modification) {
		Assert::That(clock.time(), Equals(0));
		Assert::That(!timeChanged);
		clock.setTimeCode("00:00:01:00", PhTimeCodeType25);
		Assert::That(timeChanged);
		Assert::That(time, Equals(24000));
		Assert::That(clock.time(), Equals(24000));
	}

	It(doesnt_call_rate_changed_if_no_modification) {
		Assert::That(clock.rate(), Equals(0));
		Assert::That(!rateChanged);
		Assert::That(rate, Equals(0));
		Assert::That(clock.rate(), Equals(0));
	}

	It(call_rate_changed_upon_rate_modification) {
		clock.setRate(1);
		Assert::That(rateChanged);
		Assert::That(rate, Equals(1));
		Assert::That(clock.rate(), Equals(1));
	}

	It(doesnt_call_time_changed_if_tick_with_no_rate) {
		clock.tick(1);
		Assert::That(!timeChanged);
		Assert::That(time, Equals(0));
		Assert::That(clock.time(), Equals(0));
	}

	It(call_time_changed_if_tick_with_rate) {
		clock.setRate(1);
		clock.tick(1);
		Assert::That(timeChanged);
		Assert::That(time, Equals(24000));
		Assert::That(clock.time(), Equals(24000));

		clock.setRate(-2);
		clock.tick(4);
		Assert::That(clock.time(), Equals(12000));
	}

	It(call_time_changed_if_elapsed_with_rate) {
		clock.setRate(1);
		clock.elapse(48000);
		Assert::That(timeChanged);
		Assert::That(time, Equals(48000));
		Assert::That(clock.time(), Equals(48000));

		clock.setRate(-2);
		clock.elapse(1000);
		Assert::That(clock.time(), Equals(46000));
	}
};

