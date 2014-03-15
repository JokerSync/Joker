#include <QTest>

#include "PhLockableSpinBoxTest.h"

#include "../PhLockableSpinBox.h"

PhLockableSpinBoxTest::PhLockableSpinBoxTest(QObject *parent) :
	QObject(parent)
{
}

void PhLockableSpinBoxTest::testLock()
{
	PhLockableSpinBox spinBox;

	QVERIFY(!spinBox.isEnabled());
	for(int i = 0; i < 7;i++)
		QTest::mouseClick(&spinBox, Qt::LeftButton);
	QVERIFY(!spinBox.isEnabled());
	QTest::mouseClick(&spinBox, Qt::LeftButton);
	QVERIFY(spinBox.isEnabled());
}
