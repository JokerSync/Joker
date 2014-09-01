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
	void testStripTimeChanged();
	void testStripRateChanged();
	void testVideoTimeChanged();
	void testVideoRateChanged();
	void testSyncTimeChanged();
	void testSyncRateChanged();
};

#endif // SYNCHRONIZERTEST_H
