/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef DEBUGTEST_H
#define DEBUGTEST_H

#include <QObject>

class DebugTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void init();
	void stdoutTest();
	void stderrTest();
	void logFileTest();
};

#endif // DEBUGTEST_H
