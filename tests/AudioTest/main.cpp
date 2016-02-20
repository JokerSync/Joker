/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "AudioTestWindow.h"
#include <QApplication>

#include <QSettings>

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QSettings settings(PH_ORG_NAME, PH_APP_NAME);
	QSettings::setDefaultFormat(QSettings::NativeFormat);

	QApplication a(argc, argv);
	AudioTestWindow w(&settings);
	w.show();

	return a.exec();
}
