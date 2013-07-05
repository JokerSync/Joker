/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "JokerMainWindow.h"
#include <QApplication>
#include "PhTools/PhString.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PhString file;
    if (argc < 2)
        file = "";
    else
        file = argv[1];

    JokerMainWindow w(file);
    w.show();
    
    return a.exec();
}
