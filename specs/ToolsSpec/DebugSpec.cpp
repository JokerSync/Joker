/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QMouseEvent>

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"

using namespace igloo;

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

Describe(debug_test) {
	static void SetUpContext() {
		PhDebug::enable();
	}

	void SetUp() {
		PhDebug::setDisplay(false, false, false, false, false);
		PhDebug::setLogMask(1);
	}

	It(display_in_the_debug) {
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

		Assert::That(PhDebug::getLogMask(), Equals(1));
		PHDBG(0) << "it should be displayed when default log mask is 1";
		PhDebug::setLogMask(2);
		Assert::That(PhDebug::getLogMask(), Equals(2));
		PHDBG(0) << "it should not be displayed when default log mask is 2";
		PHDBG(1) << "it should be displayed when default log mask is 2";

		QStringList lines = QString::fromStdString(buffer.str()).split("\n");
		Assert::That(lines.count(), Equals(10));
		Assert::That(QRegExp("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\.\\d\\d\\d DebugSpec.cpp\tdisplay_in_the_debug\t@[0-9]+\ttest with all log parameters").exactMatch(lines[0]));

		Assert::That(lines[1].toStdString(), Equals("test with no log parameters"));
		Assert::That(lines[2].toStdString(), Equals("shown because of showConsole(true)"));
		Assert::That(lines[3].toStdString(), Equals("shown because enable()"));
		Assert::That(lines[4].toStdString(), Equals("QEvent 0x0"));
		Assert::That(lines[5].toStdString(), Equals("QEvent MouseButtonPress"));
		Assert::That(lines[6].toStdString(), Equals("QEvent 999"));
		Assert::That(lines[7].toStdString(), Equals("it should be displayed when default log mask is 1"));
		Assert::That(lines[8].toStdString(), Equals("it should be displayed when default log mask is 2"));
	}

	It(display_in_the_error) {
		std::stringstream buffer;
		CerrRedirect redirect(buffer.rdbuf());

		PHERR << "test with no log parameters";

		PhDebug::setDisplay(true, true, true, true, true);
		PHERR << "test with all log parameters";
		QStringList lines = QString::fromStdString(buffer.str()).split("\n");
		Assert::That(lines.count(), Equals(3));
		Assert::That(lines[0].toStdString(), Equals("test with no log parameters"));
		Assert::That(QRegExp("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\.\\d\\d\\d DebugSpec.cpp\tdisplay_in_the_error\t@[0-9]+\ttest with all log parameters").exactMatch(lines[1]));
		Assert::That(lines[2].toStdString(), Equals(""));
	}
};

