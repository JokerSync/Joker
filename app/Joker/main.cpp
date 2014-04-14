/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QApplication>
#include <QDebug>
#include <QTranslator>

#include "PhStrip/PhStripDoc.h"

#include "JokerWindow.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	JokerSettings settings;
	int logMask = settings.logMask();
	PHDEBUG << ORG_NAME << APP_NAME << APP_VERSION;
	PhDebug::setLogMask(logMask);
	PhDebug::showConsole(true);

	QApplication a(argc, argv);
	QTranslator translator;
	if(!settings.language().isEmpty()) {
		PHDEBUG << "test";
		//translator.load(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + settings.language() + ".qm");
	}
	else {
		PHDEBUG << "loading default language";
		//translator.load(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + QLocale::system().name() + ".qm");
	}
	a.installTranslator(&translator);


	JokerWindow w(&settings);

	w.processArg(argc, argv);
	w.show();

	int result = a.exec();
	settings.setExitedNormaly(result == 0);


	return result;

}
