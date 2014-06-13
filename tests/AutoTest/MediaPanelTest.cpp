/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <QThread>
#include <QPushButton>
#include <QSlider>

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
	PhClock clock(PhTimeCodeType25);
	pnl.setClock(&clock);
	QVERIFY(pnl.clock() == &clock);

	pnl.show();

	pnl.setTCType(PhTimeCodeType25);
	QVERIFY(pnl.timeCodeType() == PhTimeCodeType25);
	QVERIFY(pnl.clock()->timeCodeType() == PhTimeCodeType25);

	pnl.setFirstFrame(100);
	QVERIFY(pnl.getFirstFrame() == 100);
}

void MediaPanelTest::setMediaLengthTest() {
	PhMediaPanel pnl;
	pnl.setMediaLength(100);
	QVERIFY(pnl.getMediaLength() == 100);
}

void MediaPanelTest::playPauseTest() {
	PhMediaPanel pnl;
	PhClock clock(PhTimeCodeType25);
	pnl.setClock(&clock);

	pnl.show();
	QPushButton * b = pnl.findChild<QPushButton*>("_playButton");
	QVERIFY(b);

	QVERIFY(pnl.clock()->rate() == 0);
	b->click();
	QVERIFY(pnl.clock()->rate() == 1);
	b->click();
	QVERIFY(pnl.clock()->rate() == 0);

}

void MediaPanelTest::disableSliderTest() {
	PhMediaPanel pnl;
	PhClock clock(PhTimeCodeType25);
	pnl.setClock(&clock);

	pnl.show();
	QSlider * s = pnl.findChild<QSlider*>("_slider");

	QVERIFY(s->isEnabled());
	pnl.setSliderEnable(false);
	QVERIFY(!s->isEnabled());

}

void MediaPanelTest::buttonsTest() {
	PhMediaPanel pnl;
	PhClock clock(PhTimeCodeType25);
	pnl.setClock(&clock);

	pnl.show();
	QPushButton * b = pnl.findChild<QPushButton*>("_fastForwardButton");
	QVERIFY(b);

	QVERIFY(pnl.clock()->rate() == 0);
	b->click();
	QVERIFY(pnl.clock()->rate() == 3);

	b = pnl.findChild<QPushButton*>("_playButton");
	b->click();
	QVERIFY(pnl.clock()->rate() == 0);

	b = pnl.findChild<QPushButton*>("_fastRewindButton");
	b->click();
	QVERIFY(pnl.clock()->rate() == -3);

	b = pnl.findChild<QPushButton*>("_playButton");
	b->click();
	QVERIFY(pnl.clock()->rate() == 0);

	b = pnl.findChild<QPushButton*>("_previousFrameButton");
	b->click();
	QVERIFY(pnl.clock()->frame() == -1);

	b = pnl.findChild<QPushButton*>("_nextFrameButton");
	b->click();
	QVERIFY(pnl.clock()->frame() == 0);
}
