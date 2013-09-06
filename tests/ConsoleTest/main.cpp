#include <QCoreApplication>

#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"

int main(int argc, char *argv[])
{
    // Test of PhDebug tool
    PHDEBUG << "Test of PhDebug tool";

    // Test of TimeCode
    for(int i=0; i<3;i++)
    {
        QString s = PhTimeCode::stringFromFrame(i, PhTimeCodeType25);
        int n = PhTimeCode::frameFromString(s, PhTimeCodeType25);
        PHDEBUG << s;
        if(n != i)
            PHDEBUG << "problem with " << i;
    }

    return 0;
}
