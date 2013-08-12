/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QApplication>
#include <QDebug>

#include "SDL/SDL.h"

#include "PhStrip/PhStripDoc.h"
#include "PhTools/PhString.h"
#include "PhTools/PhFileTools.h"

#include "MainView.h"
#undef main

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QString file;
    if (argc < 2)
        file = "";
    else
        file = argv[1];

    MainView w;
    w.openFile(QString(argv[1]));


    w.show();

    return a.exec();

}
