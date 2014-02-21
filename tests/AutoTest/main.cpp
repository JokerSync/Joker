#include <stdio.h>

#include <PhTools/tests/PhTimeCodeTest.h>
#include <PhStrip/tests/PhStripDocTest.h>
#include <PhSync/tests/PhSonyControllerTest.h>

int main(int argc, char *argv[])
{
	// Testing PhTimeCode
	PhTimeCodeTest tcTest;
	int result = QTest::qExec(&tcTest);
	if(result)
		return result;

	// Testing PhStripDoc
	PhStripDocTest docTest;
	result = QTest::qExec(&docTest);
	if(result)
		return result;

	// Testing PhSonyController
	PhSonyControllerTest sonyTest;
	result = QTest::qExec(&sonyTest);
	if(result)
		return result;

	return 0;
}
