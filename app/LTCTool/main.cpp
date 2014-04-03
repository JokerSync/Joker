/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "LTCToolWindow.h"
#include <QApplication>

#include "LTCToolSettings.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	LTCToolSettings settings;

	PhDebug::setLogMask(settings.logMask());

	QApplication a(argc, argv);
	LTCToolWindow w(&settings);
	w.show();

	return a.exec();
}
