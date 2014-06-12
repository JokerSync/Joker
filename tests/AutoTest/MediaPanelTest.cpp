/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <QThread>

#include "MediaPanelTest.h"

#include "PhCommonUI/PhMediaPanel.h"
#include "PhTools/PhClock.h"
#include "PhTools/PhTimeCode.h"

MediaPanelTest::MediaPanelTest(QObject *parent) :
	QObject(parent)
{
}

void MediaPanelTest::setFirstFrameTest() {
	PhMediaPanel pnl;
	pnl.show();
	pnl.setTCType(PhTimeCodeType25);
	pnl.setFirstFrame(100);
	QVERIFY(pnl.getFirstFrame() == 100);
}

void MediaPanelTest::setMediaLengthTest() {
	PhMediaPanel pnl;
	pnl.setMediaLength(100);
	QVERIFY(pnl.getMediaLength() == 100);
}


