#include <QTest>

#include "PhTimeCodeEditTest.h"

#include "../PhTimeCodeEdit.h"

PhTimeCodeEditTest::PhTimeCodeEditTest(QObject *parent) :
	QObject(parent)
{
}

void PhTimeCodeEditTest::setFrameTest()
{
	PhTimeCodeEdit tcEdit;

	tcEdit.setFrame(0, PhTimeCodeType25);
	QCOMPARE(tcEdit.text(), QString("00:00:00:00"));

}
