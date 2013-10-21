/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QApplication>

#include "PhTools/PhDebug.h"
#include "PhStrip/PhStripDoc.h"

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PhDebug::init(false, true, true, true, true, true, "VideoStripTest");

	QSettings settings("Phonations", "VideoStripTest");
	MainWindow w(&settings);

    w.show();

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

    return a.exec();

}
