/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QApplication>
#include <QDebug>

#include "PhStrip/PhStripDoc.h"
#include "PhTools/PhDebug.h"

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	PhDebug::init(false, false, true, true, true);
    w.show();

	if (argc > 1)
	{
		QString fileName = argv[1];
		w.openFile(fileName);
	}
	else
		w.createFile(1,3,3,3);

    return a.exec();
}
