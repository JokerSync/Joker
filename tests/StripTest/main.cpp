#include <QCoreApplication>
#include "phstripdoc.h"
#include "PhTools/PhString.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PhStripDoc doc("/Users/thomas/Downloads/Tunnel_Rats.detx");

    return a.exec();
}
