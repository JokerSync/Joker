#include <QCoreApplication>
#include <QTest>

#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"

#include "PhTools/tests/PhTimeCodeTest.h"
#define LVL 4

int main(int argc, char *argv[])
{

	// Initialize the Debug module.
//	PhDebug::init(false, true, true, true, true, true, 1, argv[0]);

	PHDEBUG << "should always display.";
//	PHDBG(1) << "1 should display";
//	PHDBG(2) << "2 should not display";

	QString s = "s";
	int i = 4;
	qDebug() << "test" << "test" << "test" << s << i;
	return 0;
}
