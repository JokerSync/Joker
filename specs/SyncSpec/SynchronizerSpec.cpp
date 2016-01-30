/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhSync/PhSynchronizer.h"

#include "CommonSpec.h"

#include "SyncSpecSettings.h"

using namespace bandit;

go_bandit([](){
	describe("synchronizer_test", []() {
		SyncSpecSettings *settings;
		PhSynchronizer *sync;
		PhClock *stripClock, *videoClock, *syncClock;

		before_each([&](){
			settings = new SyncSpecSettings();
			sync = new PhSynchronizer(settings);
			stripClock = new PhClock();
			videoClock = new PhClock();
			syncClock = new PhClock();
		});

		after_each([&](){
			delete syncClock;
			delete videoClock;
			delete stripClock;
			delete sync;
			delete settings;
		});

		it("set_clock", [&]() {
			sync->setStripClock(stripClock);
			sync->setVideoClock(videoClock);
			sync->setSyncClock(syncClock, PhSynchronizer::Sony);

			AssertThat(stripClock, Equals(sync->stripClock()));
			AssertThat(videoClock, Equals(sync->videoClock()));
			AssertThat(syncClock, Equals(sync->syncClock()));
		});

		it("handle_strip_time_change", [&]() {
			sync->setStripClock(stripClock);
			sync->setVideoClock(videoClock);

			stripClock->setTime(9600);

			AssertThat(stripClock->time(), Equals(9600));
			AssertThat(videoClock->time(), Equals(9600));
			AssertThat(syncClock->time(), Equals(0));

			sync->setSyncClock(syncClock, PhSynchronizer::Sony);

			stripClock->setTime(1920); // 2 frames at 25 fps

			AssertThat(stripClock->time(), Equals(1920));
			AssertThat(videoClock->time(), Equals(9600));
			AssertThat(syncClock->time(), Equals(0));
			stripClock->setTime(2880); // 3 frames at 25 fps
			AssertThat(stripClock->time(), Equals(0));
			AssertThat(videoClock->time(), Equals(9600));
			AssertThat(syncClock->time(), Equals(0));
		});

		it("handle_strip_rate_change", [&]() {
			sync->setStripClock(stripClock);
			sync->setVideoClock(videoClock);

			stripClock->setRate(1);

			AssertThat(stripClock->rate(), Equals(1));
			AssertThat(videoClock->rate(), Equals(1));
			AssertThat(syncClock->rate(), Equals(0));

			sync->setSyncClock(syncClock, PhSynchronizer::Sony);

			stripClock->setRate(2);

			AssertThat(stripClock->rate(), Equals(2));
			AssertThat(videoClock->rate(), Equals(2));
			AssertThat(syncClock->rate(), Equals(2));
		});

		it("handle_video_time_change", [&]() {
			sync->setStripClock(stripClock);
			sync->setVideoClock(videoClock);
			sync->setSyncClock(syncClock, PhSynchronizer::Sony);

			videoClock->setTime(9600);

			AssertThat(stripClock->time(), Equals(0));
			AssertThat(videoClock->time(), Equals(9600));
			AssertThat(syncClock->time(), Equals(0));
		});

		it("handle_video_rate_change", [&]() {
			sync->setStripClock(stripClock);
			sync->setVideoClock(videoClock);
			sync->setSyncClock(syncClock, PhSynchronizer::Sony);

			videoClock->setRate(3);

			AssertThat(stripClock->rate(), Equals(0));
			AssertThat(videoClock->rate(), Equals(3));
			AssertThat(syncClock->rate(), Equals(0));
		});

		it("handle_sync_time_change", [&]() {
			sync->setStripClock(stripClock);
			sync->setVideoClock(videoClock);
			sync->setSyncClock(syncClock, PhSynchronizer::Sony);

			syncClock->setTime(960);

			AssertThat(stripClock->time(), Equals(960));
			AssertThat(videoClock->time(), Equals(960));
			AssertThat(syncClock->time(), Equals(960));

			stripClock->setRate(1);
			syncClock->setTime(1920);

			AssertThat(stripClock->time(), Equals(960));
			AssertThat(videoClock->time(), Equals(1920));
			AssertThat(syncClock->time(), Equals(1920));

			syncClock->setTime(11520);

			AssertThat(stripClock->time(), Equals(11520));
			AssertThat(videoClock->time(), Equals(11520));
			AssertThat(syncClock->time(), Equals(11520));
		});

		it("handle_sync_rate_change", [&]() {
			sync->setStripClock(stripClock);
			sync->setVideoClock(videoClock);
			sync->setSyncClock(syncClock, PhSynchronizer::Sony);

			syncClock->setRate(-1);

			AssertThat(stripClock->rate(), Equals(-1));
			AssertThat(videoClock->rate(), Equals(-1));
			AssertThat(syncClock->rate(), Equals(-1));
		});

		it("returns and set time according to the clocks set", [&]() {
			stripClock->setTime(1);
			videoClock->setTime(2);
			syncClock->setTime(3);
			AssertThat(sync->time(), Equals(0));

			sync->setSyncClock(syncClock, PhSynchronizer::Sony);
			AssertThat(sync->time(), Equals(3));

			sync->setStripClock(stripClock);
			AssertThat(sync->time(), Equals(1));

			sync->setVideoClock(videoClock);
			AssertThat(sync->time(), Equals(2));

			sync->setTime(4);
			AssertThat(sync->time(), Equals(4));
			AssertThat(stripClock->time(), Equals(4));
			AssertThat(videoClock->time(), Equals(4));
			AssertThat(syncClock->time(), Equals(4));
		});

		it("returns and set rate according to the clocks set", [&]() {
			stripClock->setRate(1);
			videoClock->setRate(2);
			syncClock->setRate(3);
			AssertThat(sync->rate(), Equals(0));

			sync->setSyncClock(syncClock, PhSynchronizer::Sony);
			AssertThat(sync->rate(), Equals(3));

			sync->setStripClock(stripClock);
			AssertThat(sync->rate(), Equals(1));

			sync->setVideoClock(videoClock);
			AssertThat(sync->rate(), Equals(2));

			sync->setRate(4);
			AssertThat(sync->rate(), Equals(4));
			AssertThat(stripClock->rate(), Equals(4));
			AssertThat(videoClock->rate(), Equals(4));
			AssertThat(syncClock->rate(), Equals(4));
		});

		it("loops", [&]() {
			settings->setSyncLoopTimeIn(100);
			settings->setSyncLoopTimeOut(200);
			settings->setSyncLooping(true);

			stripClock->setTime(150);
			stripClock->setRate(1);
			sync->setStripClock(stripClock);

			AssertThat(sync->time(), Equals(150));

			stripClock->elapse(50);
			AssertThat(sync->time(), Equals(200));

			stripClock->elapse(1);
			AssertThat(sync->time(), Equals(100));
		});
	});
});
