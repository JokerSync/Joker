#ifndef PHSTRIPDOCTEST_H
#define PHSTRIPDOCTEST_H

#include <QtTest>

#include "PhStrip/PhStripDoc.h"

class PhStripDocTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void openDetXHeaderTest();
	void openDetXPeopleTest();
	void openDetXLoopTest();
	void openDetXCutTest();
	void openDetXTextTest();
	void openDetXOffTest();

private:
	PhStripDoc _doc;
};

#endif // PHSTRIPDOCTEST_H
