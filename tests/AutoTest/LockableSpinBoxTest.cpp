#include <QTest>

#include "LockableSpinBoxTest.h"

#include "PhCommonUI/PhLockableSpinBox.h"

LockableSpinBoxTest::LockableSpinBoxTest(QObject *parent) :
	QObject(parent)
{
}

void LockableSpinBoxTest::testLock()
{
	PhLockableSpinBox spinBox;

	QVERIFY(!spinBox.isEnabled());
	for(int i = 0; i < 7;i++)
		QTest::mouseClick(&spinBox, Qt::LeftButton);
	QVERIFY(!spinBox.isEnabled());
	QTest::mouseClick(&spinBox, Qt::LeftButton);
	QVERIFY(spinBox.isEnabled());
}
