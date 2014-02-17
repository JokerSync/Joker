#include <stdio.h>

#include <PhTools/tests/PhTimeCodeTest.h>
#include <PhStrip/tests/PhStripDocTest.h>

int main(int argc, char *argv[])
{
	PhTimeCodeTest tcTest;
	int result = QTest::qExec(&tcTest);
	if(result)
		return result;

	PhStripDocTest docTest;
	result = QTest::qExec(&docTest);
	if(result)
		return result;

	return 0;
}
