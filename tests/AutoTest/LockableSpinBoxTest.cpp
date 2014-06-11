#include <QTest>
#include <QThread>

#include "LockableSpinBoxTest.h"

#include "PhCommonUI/PhLockableSpinBox.h"

LockableSpinBoxTest::LockableSpinBoxTest(QObject *parent) :
	QObject(parent)
{
}

void LockableSpinBoxTest::testMultiClickUnlock()
{
	PhLockableSpinBox spinBox;

	QVERIFY(!spinBox.isEnabled());
	for(int i = 0; i < 7; i++)
	{
		QTest::mouseClick(&spinBox, Qt::LeftButton);
		QThread::msleep(300);
	}
	QVERIFY(!spinBox.isEnabled());

	for(int i = 0; i < 7; i++)
		QTest::mouseClick(&spinBox, Qt::LeftButton);
	QVERIFY(!spinBox.isEnabled());
	QTest::mouseClick(&spinBox, Qt::LeftButton);
	QVERIFY(spinBox.isEnabled());
}

void LockableSpinBoxTest::testControlClickUnload()
{
	PhLockableSpinBox spinBox;

	QVERIFY(!spinBox.isEnabled());
	QTest::mouseClick(&spinBox, Qt::LeftButton);
	QVERIFY(!spinBox.isEnabled());

	QTest::mouseClick(&spinBox, Qt::LeftButton, Qt::ControlModifier);
	QVERIFY(spinBox.isEnabled());

}
