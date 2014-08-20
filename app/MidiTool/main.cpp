/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QApplication>

#include "PhTools/PhDebug.h"

#include "MidiToolWindow.h"
#include "MidiToolSettings.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	MidiToolSettings settings;

	PhDebug::setLogMask(settings.logMask());

	QApplication a(argc, argv);
	MidiToolWindow w(&settings);
	w.show();

	return a.exec();
}
