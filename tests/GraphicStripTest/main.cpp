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
	GraphicStripTestSettings settings;
	MainWindow w(&settings);

    w.show();

	if (argc > 1)
		w.openFile(argv[1]);
	else
	{
		QString lastFile = settings.lastFile();
		if(QFile::exists(lastFile))
			w.openFile(lastFile);
		else
		{
			w.createFile(
						settings.peopleNumber(),
						settings.loopNumber(),
						settings.textNumber(),
						settings.trackNumber(),
						settings.textContent(),
						settings.startFrame()
						);
		}
	}

    return a.exec();
}
