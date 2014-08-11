/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef MIDITEST_H
#define MIDITEST_H

#include <QObject>

#include "PhSync/PhTimeCode.h"

class MidiTest : public QObject
{
	Q_OBJECT

private:
	QString t2s(PhTime time, PhTimeCodeType tcType);
	PhTime s2t(QString string, PhTimeCodeType tcType);

private slots:
	void initTestCase();
	void testConnection();
	void testQFTC();
	void testFullTC();
	void testMMCPlay();
	void testMMCStop();
	void testMMCGoto();

	void testMTC();
};

#endif // MIDITEST_H
