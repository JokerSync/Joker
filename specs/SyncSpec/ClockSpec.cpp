/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "igloo/igloo_alt.h"

#include "PhSync/PhClock.h"

using namespace igloo;

Describe(a_clock_test) {
	PhClock clock;
	It(does_nothing) {
		Assert::That(clock.time(), Equals(0));
	};
};

