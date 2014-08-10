/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef SYNCHRONIZERTEST_H
#define SYNCHRONIZERTEST_H

#include <QObject>

class SynchronizerTest : public QObject
{
	Q_OBJECT
private slots:
	void testSetClock();
	void testStripFrameChanged();
	void testStripRateChanged();
	void testVideoFrameChanged();
	void testVideoRateChanged();
	void testVideoTCTypeChanged();
	void testSyncFrameChanged();
	void testSyncRateChanged();
};

#endif // SYNCHRONIZERTEST_H
