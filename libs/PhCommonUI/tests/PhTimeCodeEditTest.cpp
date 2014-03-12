#include <QTest>
#include <QMouseEvent>

#include "PhTools/PhDebug.h"
#include "PhTimeCodeEditTest.h"

#include "../PhTimeCodeEdit.h"

PhTimeCodeEditTest::PhTimeCodeEditTest() : QObject(NULL) {
}

void PhTimeCodeEditTest::frameTest()
{
	PhTimeCodeEdit tcEdit;

	QCOMPARE((int)tcEdit.frame(), 0);

	tcEdit.setText("00:00:01:00");
	QCOMPARE((int)tcEdit.frame(), 25);
}

void PhTimeCodeEditTest::setFrameTest() {
	PhTimeCodeEdit tcEdit;

	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	tcEdit.setFrame(25, PhTimeCodeType25);
	QCOMPARE(tcEdit.text(), QString("00:00:01:00"));

	tcEdit.setFrame(48, PhTimeCodeType24);
	QCOMPARE(tcEdit.text(), QString("00:00:02:00"));
}

void PhTimeCodeEditTest::keyboardInputTest()
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

void PhTimeCodeEditTest::keyboardBadInputTest()
{
	PhTimeCodeEdit tcEdit;

	tcEdit.setFrame(25, PhTimeCodeType25);
	QCOMPARE(tcEdit.text(), QString("00:00:01:00"));

	QTest::keyClicks(&tcEdit, "a");
	QCOMPARE(tcEdit.text(), QString("00:00:01:00"));
}

void PhTimeCodeEditTest::mouseInputTest()
{
	PhTimeCodeEdit tcEdit;

	tcEdit.show();

	QTest::mousePress(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(130, 5));

	//QTest::mouseMove(&tcEdit, QPoint(130, 200)); // It doesn't seems to work use rather:
	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(130, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));

	QCOMPARE(tcEdit.text(), QString("01:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(130, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));

	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

	QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(130, 6), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));

	QCOMPARE(tcEdit.text(), QString("-01:00:00:00"));

#warning /// @todo do more mouse test

	QTest::mouseRelease(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(130, 200));
}

