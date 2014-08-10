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
	PhClock stripClock(PhTimeCodeType25), videoClock(PhTimeCodeType25), syncClock(PhTimeCodeType25);

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	QCOMPARE(&stripClock, sync.stripClock());
	QCOMPARE(&videoClock, sync.videoClock());
	QCOMPARE(&syncClock, sync.syncClock());
}

void SynchronizerTest::testStripFrameChanged()
{
	PhSynchronizer sync;
	PhClock stripClock(PhTimeCodeType25), videoClock(PhTimeCodeType25), syncClock(PhTimeCodeType25);

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);

	stripClock.setFrame(10);

	QCOMPARE((int)stripClock.frame(), 10);
	QCOMPARE((int)videoClock.frame(), 10);
	QCOMPARE((int)syncClock.frame(), 0);

	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	stripClock.setFrame(1);

	QCOMPARE((int)stripClock.frame(), 1);
	QCOMPARE((int)videoClock.frame(), 10);
	QCOMPARE((int)syncClock.frame(), 0);
	stripClock.setFrame(2);
	QCOMPARE((int)stripClock.frame(), 0);
	QCOMPARE((int)videoClock.frame(), 10);
	QCOMPARE((int)syncClock.frame(), 0);
}

void SynchronizerTest::testStripRateChanged()
{
	PhSynchronizer sync;
	PhClock stripClock(PhTimeCodeType25), videoClock(PhTimeCodeType25), syncClock(PhTimeCodeType25);

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

void SynchronizerTest::testVideoFrameChanged()
{
	PhSynchronizer sync;
	PhClock stripClock(PhTimeCodeType25), videoClock(PhTimeCodeType25), syncClock(PhTimeCodeType25);

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	videoClock.setFrame(10);

	QCOMPARE((int)stripClock.frame(), 0);
	QCOMPARE((int)videoClock.frame(), 10);
	QCOMPARE((int)syncClock.frame(), 0);
}

void SynchronizerTest::testVideoRateChanged()
{
	PhSynchronizer sync;
	PhClock stripClock(PhTimeCodeType25), videoClock(PhTimeCodeType25), syncClock(PhTimeCodeType25);

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	videoClock.setRate(3);

	QVERIFY(PhTestTools::compareFloats(stripClock.rate(), 0));
	QVERIFY(PhTestTools::compareFloats(videoClock.rate(), 3));
	QVERIFY(PhTestTools::compareFloats(syncClock.rate(), 0));
}

void SynchronizerTest::testVideoTCTypeChanged()
{
	PhSynchronizer sync;
	PhClock stripClock(PhTimeCodeType25), videoClock(PhTimeCodeType25), syncClock(PhTimeCodeType25);

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	videoClock.setTimeCodeType(PhTimeCodeType24);

	QCOMPARE(stripClock.timeCodeType(), PhTimeCodeType24);
	QCOMPARE(videoClock.timeCodeType(), PhTimeCodeType24);
	QCOMPARE(syncClock.timeCodeType(), PhTimeCodeType24);
}

void SynchronizerTest::testSyncFrameChanged()
{
	PhSynchronizer sync;
	PhClock stripClock(PhTimeCodeType25), videoClock(PhTimeCodeType25), syncClock(PhTimeCodeType25);

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	syncClock.setFrame(1);

	QCOMPARE((int)stripClock.frame(), 1);
	QCOMPARE((int)videoClock.frame(), 1);
	QCOMPARE((int)syncClock.frame(), 1);

	stripClock.setRate(1);
	syncClock.setFrame(2);

	QCOMPARE((int)stripClock.frame(), 1);
	QCOMPARE((int)videoClock.frame(), 2);
	QCOMPARE((int)syncClock.frame(), 2);

	syncClock.setFrame(12);

	QCOMPARE((int)stripClock.frame(), 12);
	QCOMPARE((int)videoClock.frame(), 12);
	QCOMPARE((int)syncClock.frame(), 12);
}

void SynchronizerTest::testSyncRateChanged()
{
	PhSynchronizer sync;
	PhClock stripClock(PhTimeCodeType25), videoClock(PhTimeCodeType25), syncClock(PhTimeCodeType25);

	sync.setStripClock(&stripClock);
	sync.setVideoClock(&videoClock);
	sync.setSyncClock(&syncClock, PhSynchronizer::Sony);

	syncClock.setRate(-1);

	QVERIFY(PhTestTools::compareFloats(stripClock.rate(), -1));
	QVERIFY(PhTestTools::compareFloats(videoClock.rate(), -1));
	QVERIFY(PhTestTools::compareFloats(syncClock.rate(), -1));
}
