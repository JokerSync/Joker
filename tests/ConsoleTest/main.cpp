#include <QCoreApplication>

#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"


int main(int argc, char *argv[])
{
    // Test of PhDebug tool
    PHDEBUG << "coucou";

    // Test of TimeCode
    for(int i=0; i<3;i++)
    {
        QString s = PhTimeCode::stringFromFrame(i, PhTimeCodeType25);
        int n = PhTimeCode::frameFromString(s, PhTimeCodeType25);
		qDebug() << s;
        if(n != i)
            qDebug() << "problem with " << i;
    }

    return 0;
}
