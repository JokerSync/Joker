/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QApplication>

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"

using namespace igloo;

int main(int argc, char *argv[])
{
	PHDEBUG << "All specs";
	QApplication a(argc, argv);
	PhDebug::disable();

	return TestRunner::RunAllTests(argc, argv);
}
