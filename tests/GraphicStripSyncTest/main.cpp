/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include <QApplication>
#include <QDebug>

#include "PhStrip/PhStripDoc.h"

#include "MainWindow.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSettings settings("Phonations", "GraphicStripSyncTest");
	MainWindow w(&settings);

	if (argc > 1) {
		QString fileName = argv[1];
		w.openFile(fileName);
	}
	else{
		QString lastFile = settings.value("lastFile", "").toString();
		if(QFile(lastFile).exists())
			w.openFile(lastFile);
	}

	w.show();

	return a.exec();

}
