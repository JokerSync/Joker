/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QApplication>
#include <QDebug>
#include <QSettings>

#include "PhStrip/PhStripDoc.h"

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QSettings settings("Phonations", "Joker");
	QSettings::setDefaultFormat(QSettings::NativeFormat);
	int logLevel = settings.value("logLevel", 1).toInt();
	PHDEBUG << ORG_NAME << APP_NAME << APP_VERSION;
	PhDebug::setLogMask(logLevel);

	QApplication a(argc, argv);

	MainWindow w(&settings);

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
