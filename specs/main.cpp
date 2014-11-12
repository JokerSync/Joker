/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhGeneric.h"

#include "PhTools/PhDebug.h"

#include "PhSpec.h"

int main(int argc, char *argv[])
{
	PHDEBUG << "All specs";
	QApplication a(argc, argv);

	return bandit::run(argc, argv);
}
