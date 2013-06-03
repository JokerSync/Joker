#include <QApplication>
#include <QDebug>

#include "SDL/SDL.h"

#include "PhStrip/PhStripDoc.h"
#include "PhTools/PhString.h"

#include "PhGraphic/MainWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PhString file;
    if (argc < 2)
        file = "";
    else
        file = argv[1];

    MainWindow w(file);

    w.show();

    return a.exec();
}
