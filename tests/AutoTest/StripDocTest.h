/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPDOCTEST_H
#define PHSTRIPDOCTEST_H

#include <QtTest>

#include "PhStrip/PhStripDoc.h"

class StripDocTest : public QObject
{
	Q_OBJECT
private slots:
	// Import DetX tests
	void importDetXHeaderTest();
	void importDetXNoFile();
	void importDetXPeopleTest();
	void importDetXLoopTest();
	void importDetXCutTest();
	void importDetXTextTest();
	void importDetXDetectTest();
	void importDetXNoTitleTest();

	// Import Mos tests
	void importMosTest01();
	void importMosTest02();
	void importMosTest03();

	// Open any doc (*.detx, *.strip, *.joker) test
	void openStripFileTest();

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
	QString f2s(PhFrame frame, PhTimeCodeType tcType = PhTimeCodeType25);
	PhFrame s2f(QString string, PhTimeCodeType tcType = PhTimeCodeType25);
};

#endif // PHSTRIPDOCTEST_H
