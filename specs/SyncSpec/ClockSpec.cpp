/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhSync/PhClock.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("clock_test", []() {
		PhClock clock;
		PhTime time;
		PhRate rate;
		bool timeChanged;
		bool rateChanged;

		before_each([&](){
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
		});

		it("doesnt_call_time_changed_if_no_modification", [&](){
			AssertThat(clock.time(), Equals(0));
			AssertThat(timeChanged, IsFalse());
			AssertThat(time, Equals(0));
			AssertThat(clock.time(), Equals(0));
		});

		it("call_time_changed_upon_time_modification", [&](){
			clock.setTime(1);
			AssertThat(timeChanged, IsTrue());
			AssertThat(time, Equals(1));
			AssertThat(clock.time(), Equals(1));
		});

		it("call_time_changed_upon_frame_modification", [&](){
			AssertThat(clock.time(), Equals(0));
			AssertThat(timeChanged, IsFalse());
			clock.setFrame(10, PhTimeCodeType25);
			AssertThat(timeChanged, IsTrue());
			AssertThat(time, Equals(9600));
			AssertThat(clock.time(), Equals(9600));
		});

		it("call_time_changed_upon_millisecond_modification", [&](){
			AssertThat(clock.time(), Equals(0));
			AssertThat(timeChanged, IsFalse());
			clock.setMillisecond(1000);
			AssertThat(timeChanged, IsTrue());
			AssertThat(time, Equals(24000));
			AssertThat(clock.time(), Equals(24000));
		});

		it("call_time_changed_upon_timecode_modification", [&](){
			AssertThat(clock.time(), Equals(0));
			AssertThat(timeChanged, IsFalse());
			clock.setTimeCode("00:00:01:00", PhTimeCodeType25);
			AssertThat(timeChanged, IsTrue());
			AssertThat(time, Equals(24000));
			AssertThat(clock.time(), Equals(24000));
		});

		it("doesnt_call_rate_changed_if_no_modification", [&](){
			AssertThat(clock.rate(), Equals(0));
			AssertThat(rateChanged, IsFalse());
			AssertThat(rate, Equals(0));
			AssertThat(clock.rate(), Equals(0));
		});

		it("call_rate_changed_upon_rate_modification", [&](){
			clock.setRate(1);
			AssertThat(rateChanged, IsTrue());
			AssertThat(rate, Equals(1));
			AssertThat(clock.rate(), Equals(1));
		});

		it("doesnt_call_time_changed_if_elapsed_with_no_rate", [&](){
			clock.elapse(24000);
			AssertThat(timeChanged, IsFalse());
			AssertThat(time, Equals(0));
			AssertThat(clock.time(), Equals(0));
		});

		it("call_time_changed_if_elapsed_with_rate", [&](){
			clock.setRate(1);
			clock.elapse(48000);
			AssertThat(timeChanged, IsTrue());
			AssertThat(time, Equals(48000));
			AssertThat(clock.time(), Equals(48000));

			clock.setRate(-2);
			clock.elapse(1000);
			AssertThat(clock.time(), Equals(46000));
		});
	});
});


