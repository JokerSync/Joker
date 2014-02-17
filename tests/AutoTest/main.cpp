#include <stdio.h>

#include <PhTools/tests/PhTimeCodeTest.h>

int main(int argc, char *argv[])
{
	PhTimeCodeTest tcTest;
	int result = QTest::qExec(&tcTest);
	if(result)
		return result;

	return 0;
}
