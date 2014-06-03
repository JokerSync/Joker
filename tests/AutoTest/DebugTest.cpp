/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> // std::stringstream

#include <QMouseEvent>
#include <QDir>

#include "PhTools/PhDebug.h"

#include "DebugTest.h"


class CoutRedirect {
public:
	CoutRedirect( std::streambuf * new_buffer )
		: old( std::cout.rdbuf( new_buffer ) )
	{
	}

	~CoutRedirect() {
		std::cout.rdbuf( old );
	}
private:
	std::streambuf * old;
};

class CerrRedirect {
public:
	CerrRedirect( std::streambuf * new_buffer )
		: old( std::cerr.rdbuf( new_buffer ) )
	{
	}

	~CerrRedirect() {
		std::cerr.rdbuf( old );
	}
private:
	std::streambuf * old;
};

void DebugTest::initTestCase()
{
	PhDebug::enable();
}

void DebugTest::init()
{
	PhDebug::setDisplay(false, false, false, false, false);
	PhDebug::setLogMask(1);
}

void DebugTest::stdoutTest()
{
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

	QEvent * event = NULL;
	PHDEBUG << event;
	event = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	PHDEBUG << event;
	delete event;
	event = new QEvent((QEvent::Type)999);
	PHDEBUG << event;

	QCOMPARE(PhDebug::getLogMask(), 1);
	PHDBG(0) << "it should be displayed when default log mask is 1";
	PhDebug::setLogMask(2);
	QCOMPARE(PhDebug::getLogMask(), 2);
	PHDBG(0) << "it should not be displayed when default log mask is 2";
	PHDBG(1) << "it should be displayed when default log mask is 2";

	QStringList lines = QString::fromStdString(buffer.str()).split("\n");
	QCOMPARE(lines.count(), 10);
	QVERIFY2(QRegExp("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\.\\d\\d\\d DebugTest.cpp\tstdoutTest\t@[0-9]+\ttest with all log parameters ").exactMatch(lines[0]), PHNQ(lines[0]));
	QCOMPARE(lines[1], QString("test with no log parameters "));
	QCOMPARE(lines[2], QString("shown because of showConsole(true) "));
	QCOMPARE(lines[3], QString("shown because enable() "));
	QCOMPARE(lines[4], QString("QEvent 0x0 "));
	QCOMPARE(lines[5], QString("QEvent MouseButtonPress "));
	QCOMPARE(lines[6], QString("QEvent 999 "));
	QCOMPARE(lines[7], QString("it should be displayed when default log mask is 1 "));
	QCOMPARE(lines[8], QString("it should be displayed when default log mask is 2 "));
}

void DebugTest::stderrTest()
{
	std::stringstream buffer;

	CerrRedirect redirect(buffer.rdbuf());

	PHERR << "test with no log parameters";

	PhDebug::setDisplay(true, true, true, true, true);
	PHERR << "test with all log parameters";

	QStringList lines = QString::fromStdString(buffer.str()).split("\n");
	QCOMPARE(lines.count(), 3);
	QVERIFY2(QRegExp("test with no log parameters ").exactMatch(lines[0]), PHNQ(lines[0]));
	QVERIFY2(QRegExp("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\.\\d\\d\\d DebugTest.cpp\tstderrTest\t@[0-9]+\ttest with all log parameters ").exactMatch(lines[1]), PHNQ(lines[1]));
	QCOMPARE(lines[2], QString(""));
}

void DebugTest::logFileTest()
{
	QString expectedLogLocation = QString("%1/Library/Logs/Phonations/AutoTest.log")
	                              .arg(QString(QDir::homePath()));
	QCOMPARE(PhDebug::logLocation(), expectedLogLocation);

	QFile log(expectedLogLocation);
	PHDEBUG << "last line in the log";
	QVERIFY(log.open(QFile::ReadOnly));
	QStringList lines = QTextStream(&log).readAll().split("\n");
	QVERIFY(lines.count() >= 2);

	QCOMPARE(lines[lines.count() - 2], QString("last line in the log "));
}
