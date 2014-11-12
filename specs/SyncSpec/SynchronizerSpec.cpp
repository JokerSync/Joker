/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhSync/PhSynchronizer.h"

#include "PhSpec.h"

using namespace bandit;

go_bandit([](){
	describe("synchronizer_test", []() {
		it("set_clock", []() {
			PhSynchronizer sync;
			PhClock stripClock, videoClock, syncClock;

			sync.setStripClock(&stripClock);
			sync.setVideoClock(&videoClock);
			sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

			AssertThat(&stripClock, Equals(sync.stripClock()));
			AssertThat(&videoClock, Equals(sync.videoClock()));
			AssertThat(&syncClock, Equals(sync.syncClock()));
		});

		it("handle_strip_time_change", []() {
			PhSynchronizer sync;
			PhClock stripClock, videoClock, syncClock;

			sync.setStripClock(&stripClock);
			sync.setVideoClock(&videoClock);

			stripClock.setTime(9600);

			AssertThat(stripClock.time(), Equals(9600));
			AssertThat(videoClock.time(), Equals(9600));
			AssertThat(syncClock.time(), Equals(0));

			sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

			stripClock.setTime(1920); // 2 frames at 25 fps

			AssertThat(stripClock.time(), Equals(1920));
			AssertThat(videoClock.time(), Equals(9600));
			AssertThat(syncClock.time(), Equals(0));
			stripClock.setTime(2880); // 3 frames at 25 fps
			AssertThat(stripClock.time(), Equals(0));
			AssertThat(videoClock.time(), Equals(9600));
			AssertThat(syncClock.time(), Equals(0));
		});

		it("handle_strip_rat_e_change", []() {
			PhSynchronizer sync;
			PhClock stripClock, videoClock, syncClock;

			sync.setStripClock(&stripClock);
			sync.setVideoClock(&videoClock);

			stripClock.setRate(1);

			AssertThat(stripClock.rate(), Equals(1));
			AssertThat(videoClock.rate(), Equals(1));
			AssertThat(syncClock.rate(), Equals(0));

			sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

			stripClock.setRate(2);

			AssertThat(stripClock.rate(), Equals(2));
			AssertThat(videoClock.rate(), Equals(2));
			AssertThat(syncClock.rate(), Equals(2));
		});

		it("handle_video_time_change", []() {
			PhSynchronizer sync;
			PhClock stripClock, videoClock, syncClock;

			sync.setStripClock(&stripClock);
			sync.setVideoClock(&videoClock);
			sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

			videoClock.setTime(9600);

			AssertThat(stripClock.time(), Equals(0));
			AssertThat(videoClock.time(), Equals(9600));
			AssertThat(syncClock.time(), Equals(0));
		});

		it("handle_video_rate_change", []() {
			PhSynchronizer sync;
			PhClock stripClock, videoClock, syncClock;

			sync.setStripClock(&stripClock);
			sync.setVideoClock(&videoClock);
			sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

			videoClock.setRate(3);

			AssertThat(stripClock.rate(), Equals(0));
			AssertThat(videoClock.rate(), Equals(3));
			AssertThat(syncClock.rate(), Equals(0));
		});

		it("handle_sync_time_change", []() {
			PhSynchronizer sync;
			PhClock stripClock, videoClock, syncClock;

			sync.setStripClock(&stripClock);
			sync.setVideoClock(&videoClock);
			sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

			syncClock.setTime(960);

			AssertThat(stripClock.time(), Equals(960));
			AssertThat(videoClock.time(), Equals(960));
			AssertThat(syncClock.time(), Equals(960));

			stripClock.setRate(1);
			syncClock.setTime(1920);

			AssertThat(stripClock.time(), Equals(960));
			AssertThat(videoClock.time(), Equals(1920));
			AssertThat(syncClock.time(), Equals(1920));

			syncClock.setTime(11520);

			AssertThat(stripClock.time(), Equals(11520));
			AssertThat(videoClock.time(), Equals(11520));
			AssertThat(syncClock.time(), Equals(11520));
		});

		it("handle_sync_rae_change", []() {
			PhSynchronizer sync;
			PhClock stripClock, videoClock, syncClock;

			sync.setStripClock(&stripClock);
			sync.setVideoClock(&videoClock);
			sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

			syncClock.setRate(-1);

			AssertThat(stripClock.rate(), Equals(-1));
			AssertThat(videoClock.rate(), Equals(-1));
			AssertThat(syncClock.rate(), Equals(-1));
		});
	});
});
