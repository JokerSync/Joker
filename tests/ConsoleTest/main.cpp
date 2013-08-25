#include <QCoreApplication>

#include <QDebug>

#include "PhTools/PhTimeCode.h"

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    for(int i=0; i<2000000;i++)
    {
        QString s = PhTimeCode::stringFromFrame(i, PhTimeCodeType25);
        int n = PhTimeCode::frameFromString(s, PhTimeCodeType25);
        if(n != i)
            qDebug() << "problem with " << i;
    }
    //return a.exec();
    return 0;
}
