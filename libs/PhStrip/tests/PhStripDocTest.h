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
	// Import DetX tests
	void importDetXHeaderTest();
	void importDetXPeopleTest();
	void importDetXLoopTest();
	void importDetXCutTest();
	void importDetXTextTest();
	void importDetXOffTest();

	// Get people
	void getPeopleByNameTest();

	// Get prev/next element tests
	void getPreviousElementFrameTest();
	void getNextElementFrameTest();
	void getNextTextTest();
	void getNextTextTestByPeople();
	void getNextTextTestByPeopleList();

private:
	QString f2s(PhFrame frame);
	PhFrame s2f(QString string);

	PhStripDoc _doc;
};

#endif // PHSTRIPDOCTEST_H
