/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhSony/PhSonyController.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("sony_controller_test", []() {
		it("compute_rate_from_data_bytes", []() {
			AssertThat(PhSonyController::computeRate(0), EqualsWithDelta(0.0f, 0.01f));
			AssertThat(PhSonyController::computeRate(32), EqualsWithDelta(0.1f, 0.01f));
			AssertThat(PhSonyController::computeRate(64), EqualsWithDelta(1.0f, 0.01f));
			AssertThat(PhSonyController::computeRate(79), EqualsWithDelta(2.94f, 0.01f));
			AssertThat(PhSonyController::computeRate(118), EqualsWithDelta(48.69f, 0.01f));
		});
	});
});
