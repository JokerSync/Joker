/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QTest>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> // std::stringstream

#include "PhTools/PhDebug.h"

#include "DebugTest.h"

#include <QMouseEvent>

class CoutRedirect {
public:
	CoutRedirect( std::streambuf * new_buffer )
		: old( std::cout.rdbuf( new_buffer ) )
	{ }

	~CoutRedirect( ) {
		std::cout.rdbuf( old );
	}
private:
	std::streambuf * old;
};

class CerrRedirect {
public:
	CerrRedirect( std::streambuf * new_buffer )
		: old( std::cerr.rdbuf( new_buffer ) )
	{ }

	~CerrRedirect( ) {
		std::cerr.rdbuf( old );
	}
private:
	std::streambuf * old;
};

void DebugTest::initTestCase()
{
	PhDebug::enable();
}

void DebugTest::stdoutTest()
{
	QCOMPARE(PhDebug::getLogMask(), 1);

	std::stringstream buffer;
	CoutRedirect redirect(buffer.rdbuf());

	PhDebug::setDisplay(true, true, true, true, true);
	PHDEBUG << "test with all log parameters";

	PhDebug::setDisplay(false, false, false, false, false);
	PHDEBUG << "test with no log parameters";

	PhDebug::showConsole(false);
	PHDEBUG << "not shown because of showConsole(false)";

	PhDebug::showConsole(true);
	PHDEBUG << "shown because of showConsole(true)";

	PhDebug::disable();
	PHDEBUG << "not shown because disable()";

	PhDebug::enable();
	PHDEBUG << "shown because enable()";

	QMouseEvent event(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	PHDEBUG << &event;

	QStringList lines = QString::fromStdString(buffer.str()).split("\n");
	QCOMPARE(lines.count(), 6);
	QVERIFY2(QRegExp("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\.\\d\\d\\d DebugTest.cpp\tstdoutTest\t@\\d\\d\ttest with all log parameters ").exactMatch(lines[0]), PHNQ(lines[0]));
	QCOMPARE(lines[1], QString("test with no log parameters "));
	QCOMPARE(lines[2], QString("shown because of showConsole(true) "));
	QCOMPARE(lines[3], QString("shown because enable() "));
	QCOMPARE(lines[4], QString("QEvent \"MouseButtonPress\"  "));
	QCOMPARE(lines[5], QString(""));
}

void DebugTest::stderrTest()
{
	std::stringstream buffer;

	CerrRedirect redirect(buffer.rdbuf());

	PhDebug::setDisplay(false, false, false, false, false);
	PHERR << "test with no log parameters";

	PhDebug::setDisplay(true, true, true, true, true);
	PHERR << "test with all log parameters";

	QStringList lines = QString::fromStdString(buffer.str()).split("\n");
	QCOMPARE(lines.count(), 3);
	QVERIFY2(QRegExp("test with no log parameters ").exactMatch(lines[0]), PHNQ(lines[0]));
	QVERIFY2(QRegExp("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\.\\d\\d\\d DebugTest.cpp\tstderrTest\t@\\d\\d\ttest with all log parameters ").exactMatch(lines[1]), PHNQ(lines[1]));
	QCOMPARE(lines[2], QString(""));
}
