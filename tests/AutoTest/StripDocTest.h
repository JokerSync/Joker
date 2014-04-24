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
	void initTestCase();

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
	void importMosTest04();

	// Open any doc (*.detx, *.strip, *.joker) test
	void openStripFileTest();
	void openSaveTest01();
	void openSaveTest02();

	// Get people
	void getPeopleByNameTest();

	// Get prev/next element tests
	void getPreviousElementTimeTest();
	void getNextElementTimeTest();
	void getNextTextTest();
	void getNextTextTestByPeople();
	void getNextTextTestByPeopleList();
	void getNextLoopTest();
	void getPreviousLoopTest();

	void addObjectTest();
	void addPeopleTest();

private:
	QString t2s(PhTime time, PhTimeCodeType tcType);
	PhTime s2t(QString string, PhTimeCodeType tcType);
};

#endif // PHSTRIPDOCTEST_H
