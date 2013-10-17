#include <QCoreApplication>
#include <QTest>

#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"

#include "PhTools/tests/PhTimeCodeTest.h"

int main(int argc, char *argv[])
{
	// Initialize the Debug module.
	PhDebug::init(false, true, true, true, true, false, argv[0]);

    // Test of PhDebug tool
    PHDEBUG << "Test of PhDebug tool";

    // Test of TimeCode
    for(int i=0; i<3;i++)
    {
        QString s = PhTimeCode::stringFromFrame(i, PhTimeCodeType25);
        int n = PhTimeCode::frameFromString(s, PhTimeCodeType25);
        //PHDEBUG << s;
        if(n != i)
            PHDEBUG << "problem with " << i;
    }

    PhTimeCodeTest tcTest;

	return QTest::qExec(&tcTest);
}
