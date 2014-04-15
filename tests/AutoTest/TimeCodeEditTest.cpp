#include <QTest>
#include <QMouseEvent>

#include "PhTools/PhDebug.h"
#include "TimeCodeEditTest.h"

#include "PhCommonUI/PhTimeCodeEdit.h"

TimeCodeEditTest::TimeCodeEditTest() : QObject(NULL) {
}

void TimeCodeEditTest::frameTest()
{
	PhTimeCodeEdit tcEdit;

	QCOMPARE((int)tcEdit.frame(), 0);

	tcEdit.setText("00:00:01:00");
	QCOMPARE((int)tcEdit.frame(), 25);
}

void TimeCodeEditTest::setFrameTest() {
	PhTimeCodeEdit tcEdit;

	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	tcEdit.setFrame(25, PhTimeCodeType25);
	QCOMPARE(tcEdit.text(), QString("00:00:01:00"));

	tcEdit.setFrame(48, PhTimeCodeType24);
	QCOMPARE(tcEdit.text(), QString("00:00:02:00"));
}

void TimeCodeEditTest::keyboardInputTest()
{
	PhTimeCodeEdit tcEdit;

	QTest::keyClicks(&tcEdit, "9");
	QCOMPARE(tcEdit.text(), QString("00:00:00:09"));
	QCOMPARE((int)tcEdit.frame(), 9);
	QVERIFY(tcEdit.isTimeCode());

	QTest::keyClicks(&tcEdit, "1");
	QCOMPARE(tcEdit.text(), QString("00:00:00:91"));
	QCOMPARE((int)tcEdit.frame(), 0);
	QVERIFY(!tcEdit.isTimeCode());

	QTest::keyClicks(&tcEdit, "2");
	QCOMPARE(tcEdit.text(), QString("00:00:09:12"));
	QCOMPARE((int)tcEdit.frame(), 9 * 25 + 12);
	QVERIFY(tcEdit.isTimeCode());

	QTest::keyClick(&tcEdit, Qt::Key_Backspace);
	QCOMPARE(tcEdit.text(), QString("00:00:00:91"));
	QCOMPARE((int)tcEdit.frame(), 0);
	QVERIFY(!tcEdit.isTimeCode());

	QTest::keyClick(&tcEdit, Qt::Key_Enter);
	QCOMPARE(tcEdit.text(), QString("00:00:00:91"));
}

void TimeCodeEditTest::keyboardBadInputTest()
{
	PhTimeCodeEdit tcEdit;

	tcEdit.setFrame(25, PhTimeCodeType25);
	QCOMPARE(tcEdit.text(), QString("00:00:01:00"));

	QTest::keyClicks(&tcEdit, "a");
	QCOMPARE(tcEdit.text(), QString("00:00:01:00"));
}

void TimeCodeEditTest::mouseInputTest()
{
	PhTimeCodeEdit tcEdit;

	tcEdit.show();

	// Hour testing
	// Vertical axis mouse move
	QTest::mousePress(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(130, 5));

	//QTest::mouseMove(&tcEdit, QPoint(130, 200)); // It doesn't seems to work use rather:
	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(130, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("01:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(130, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(130, 6), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("-01:00:00:00"));

	// Vertical and Horizontal axis mouse move, horizontal moves are out
	// of the text limits
	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(100, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(200, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("01:00:00:00"));

	QTest::mouseRelease(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(130, 200));

	//Reset
	tcEdit.setText("00:00:00:00");
	// Minutes testing
	// Vertical axis mouse move
	QTest::mousePress(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(167, 5), 100);

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(167, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:01:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(167, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(167, 6), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("-00:01:00:00"));

	// Vertical and Horizontal axis mouse move, horizontal moves are out
	// of the text limits
	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(100, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(200, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:01:00:00"));

	QTest::mouseRelease(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(130, 200));

	//Reset
	tcEdit.setText("00:00:00:00");
	// Seconds testing
	// Vertical axis mouse move
	QTest::mousePress(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(167, 5), 100);

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(167, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:01:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(167, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(167, 6), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("-00:01:00:00"));

	// Vertical and Horizontal axis mouse move, horizontal moves are out
	// of the text limits
	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(100, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(200, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:01:00:00"));

	QTest::mouseRelease(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(130, 200));


	//Reset
	tcEdit.setText("00:00:00:00");
	// frames testing
	// Vertical axis mouse move
	QTest::mousePress(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(250, 5), 100);

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(250, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:01"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(250, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(250, 6), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("-00:00:00:01"));

	// Vertical and Horizontal axis mouse move, horizontal moves are out
	// of the text limits
	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(100, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(200, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
	QCOMPARE(tcEdit.text(), QString("00:00:00:01"));

	QTest::mouseRelease(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(250, 200));
#warning /// @todo do more mouse test

}

