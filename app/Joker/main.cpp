/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QApplication>
#include <QDebug>
#include <QSettings>

#include "PhStrip/PhStripDoc.h"

#include "JokerWindow.h"

int main(int argc, char *argv[])
{
	QSettings settings("Phonations", "Joker");
	QSettings::setDefaultFormat(QSettings::NativeFormat);
	int logMask = settings.value("logMask", 1).toInt();
	PHDEBUG << ORG_NAME << APP_NAME << APP_VERSION;
	PhDebug::setLogMask(logMask);
	PhDebug::showConsole(true);

	QApplication a(argc, argv);
    QTranslator translator;
	translator.load(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + QLocale::system().name() + ".qm");
    a.installTranslator(&translator);


	JokerWindow w(&settings);

	w.show();

	QString fileName = "";
	if (argc > 1)
		fileName = argv[1];
	else if(settings.value("openLastFile", false).toBool()) // Load the last file if the setting si selected
		fileName = settings.value("lastFile").toString();

	if(QFile(fileName).exists())
		w.openFile(fileName);

	return a.exec();

}
