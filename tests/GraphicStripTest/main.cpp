/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QApplication>
#include <QDebug>
#include <QSettings>

#include "PhStrip/PhStripDoc.h"
#include "PhTools/PhDebug.h"

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSettings settings("Phonations","GraphicStripTest");
	MainWindow w(&settings);

	PhDebug::init(false, false, true, true, true);
    w.show();

	if (argc > 1)
		w.openFile(argv[1]);
	else
	{
		QString lastFile = settings.value("lastFile", "").toString();
		if(QFile::exists(lastFile))
			w.openFile(lastFile);
		else
		{
			w.createFile(
						settings.value("peopleNumber", 0).toInt(),
						settings.value("loopNumber", 0).toInt(),
						settings.value("textNumber", 0).toInt(),
						settings.value("trackNumber", 4).toInt(),
						settings.value("textContent", "Per hoc minui studium suum existimans Paulus.").toString(),
						settings.value("startFrame", 90000).toInt()
						);
		}
	}

    return a.exec();
}
