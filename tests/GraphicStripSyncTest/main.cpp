/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QApplication>
#include <QDebug>

#include "PhStrip/PhStripDoc.h"

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSettings settings("Phonations", "GraphicStripSyncTest");
	MainWindow w(&settings);

	if (argc > 1)
	{
		QString fileName = argv[1];
		w.openFile(fileName);
	}
	else
	{
		QString lastFile = settings.value("lastFile", "").toString();
		if(QFile(lastFile).exists())
			w.openFile(lastFile);
	}

    w.show();

    return a.exec();

}
