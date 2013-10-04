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
	MainWindow w;

	if (argc > 1)
	{
		QString fileName = argv[1];
		w.openFile(fileName);
	}

    w.show();

    return a.exec();

}
