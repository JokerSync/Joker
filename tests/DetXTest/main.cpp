/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QCoreApplication>


#include "phstripdoc.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PhStripDoc *doc = new PhStripDoc(QString(""));
    return a.exec();
}
