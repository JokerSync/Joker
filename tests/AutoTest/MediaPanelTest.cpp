/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <QThread>

#include "MediaPanelTest.h"

#include "PhCommonUI/PhMediaPanel.h"
#include "PhTools/PhClock.h"

MediaPanelTest::MediaPanelTest(QObject *parent) :
	QObject(parent)
{
}

void MediaPanelTest::setFirstFrameTest() {
	PhMediaPanel pnl;
	pnl.show();
	pnl.setFirstFrame(100);
	QVERIFY(pnl.getFirstFrame() == 100);
}


