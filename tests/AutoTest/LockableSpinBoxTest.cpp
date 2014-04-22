#include <QTest>

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

	QTest::keyPress(&spinBox, Qt::ControlModifier);
	QTest::mouseClick(&spinBox, Qt::LeftButton);
	QVERIFY(!spinBox.isEnabled());
}
