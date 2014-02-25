/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPDOCTEST_H
#define PHSTRIPDOCTEST_H

#include <QtTest>

#include "PhStrip/PhStripDoc.h"

class PhStripDocTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Open DetX tests
	void openDetXHeaderTest();
	void openDetXPeopleTest();
	void openDetXLoopTest();
	void openDetXCutTest();
	void openDetXTextTest();
	void openDetXDetectTest();

	// Get people
	void getPeopleByNameTest();

	// Get prev/next element tests
	void getPreviousElementFrameTest();
	void getNextElementFrameTest();
	void getNextTextTest();
	void getNextTextTestByPeople();
	void getNextTextTestByPeopleList();
	void getNextLoopTest();
	void getPreviousLoopTest();

private:
	QString f2s(PhFrame frame);
	PhFrame s2f(QString string);

	PhStripDoc _doc;
};

#endif // PHSTRIPDOCTEST_H
