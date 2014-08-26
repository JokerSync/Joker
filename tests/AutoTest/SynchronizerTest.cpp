/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "PhTools/PhTestTools.h"
#include "PhSync/PhSynchronizer.h"

#include "SynchronizerTest.h"

void SynchronizerTest::testSetClock()
{
	PhSynchronizer sync;
	PhClock stripClock, videoClock, syncClock;

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	QCOMPARE(&stripClock, sync.stripClock());
	QCOMPARE(&videoClock, sync.videoClock());
	QCOMPARE(&syncClock, sync.syncClock());
}

void SynchronizerTest::testStripTimeChanged()
{
	PhSynchronizer sync;
	PhClock stripClock, videoClock, syncClock;

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);

	stripClock.setTime(9600);

	QCOMPARE((int)stripClock.time(), 9600);
	QCOMPARE((int)videoClock.time(), 9600);
	QCOMPARE((int)syncClock.time(), 0);

	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	stripClock.setTime(960);

	QCOMPARE((int)stripClock.time(), 960);
	QCOMPARE((int)videoClock.time(), 9600);
	QCOMPARE((int)syncClock.time(), 0);
	stripClock.setTime(1920);
	QCOMPARE((int)stripClock.time(), 0);
	QCOMPARE((int)videoClock.time(), 9600);
	QCOMPARE((int)syncClock.time(), 0);
}

void SynchronizerTest::testStripRateChanged()
{
	PhSynchronizer sync;
	PhClock stripClock, videoClock, syncClock;

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);

	stripClock.setRate(1);

	QVERIFY(PhTestTools::compareFloats(stripClock.rate(), 1));
	QVERIFY(PhTestTools::compareFloats(videoClock.rate(), 1));
	QVERIFY(PhTestTools::compareFloats(syncClock.rate(), 0));

	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	stripClock.setRate(2);

	QVERIFY(PhTestTools::compareFloats(stripClock.rate(), 2));
	QVERIFY(PhTestTools::compareFloats(videoClock.rate(), 2));
	QVERIFY(PhTestTools::compareFloats(syncClock.rate(), 2));
}

void SynchronizerTest::testVideoTimeChanged()
{
	PhSynchronizer sync;
	PhClock stripClock, videoClock, syncClock;

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	videoClock.setTime(9600);

	QCOMPARE((int)stripClock.time(), 0);
	QCOMPARE((int)videoClock.time(), 9600);
	QCOMPARE((int)syncClock.time(), 0);
}

void SynchronizerTest::testVideoRateChanged()
{
	PhSynchronizer sync;
	PhClock stripClock, videoClock, syncClock;

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	videoClock.setRate(3);

	QVERIFY(PhTestTools::compareFloats(stripClock.rate(), 0));
	QVERIFY(PhTestTools::compareFloats(videoClock.rate(), 3));
	QVERIFY(PhTestTools::compareFloats(syncClock.rate(), 0));
}

void SynchronizerTest::testSyncTimeChanged()
{
	PhSynchronizer sync;
	PhClock stripClock, videoClock, syncClock;

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	syncClock.setTime(960);

	QCOMPARE((int)stripClock.time(), 960);
	QCOMPARE((int)videoClock.time(), 960);
	QCOMPARE((int)syncClock.time(), 960);

	stripClock.setRate(1);
	syncClock.setTime(1920);

	QCOMPARE((int)stripClock.time(), 960);
	QCOMPARE((int)videoClock.time(), 1920);
	QCOMPARE((int)syncClock.time(), 1920);

	syncClock.setTime(11520);

	QCOMPARE((int)stripClock.time(), 11520);
	QCOMPARE((int)videoClock.time(), 11520);
	QCOMPARE((int)syncClock.time(), 11520);
}

void SynchronizerTest::testSyncRateChanged()
{
	PhSynchronizer sync;
	PhClock stripClock, videoClock, syncClock;

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	syncClock.setRate(-1);

	QVERIFY(PhTestTools::compareFloats(stripClock.rate(), -1));
	QVERIFY(PhTestTools::compareFloats(videoClock.rate(), -1));
	QVERIFY(PhTestTools::compareFloats(syncClock.rate(), -1));
}
