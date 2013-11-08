#include <QCoreApplication>
#include <QTest>

#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"

#include "PhTools/tests/PhTimeCodeTest.h"
#define LVL 4


int main(int argc, char *argv[])
{

	// Initialize the Debug module.
	PhDebug::init(false, true, true, true, true, true, 8, argv[0]);
    // Test of PhDebug tool
	//PHDEBUG(0x01) << "Test of PhDebug tool";

    // Test of TimeCode
    for(int i=0; i<3;i++)
    {
        QString s = PhTimeCode::stringFromFrame(i, PhTimeCodeType25);
        int n = PhTimeCode::frameFromString(s, PhTimeCodeType25);
        //PHDEBUG << s;
        if(n != i)
			PHDEBUG(0x10) << "problem with " << i;
    }

	for (int i = 0; i < 10; i++)
		PHDEBUG(i) << i;

	PHDEBUG(LVL) << 0x100;


	//PhTimeCodeTest tcTest;

	return 0;//QTest::qExec(&tcTest);
}
