/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include <QApplication>

#include "PhStrip/PhStripDoc.h"
#include "PhTools/PhDebug.h"

#include "GraphicStripTestWindow.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GraphicStripTestSettings settings;
	GraphicStripTestWindow w(&settings);
	w.processArg(argc, argv);
	w.show();

	return a.exec();
}
