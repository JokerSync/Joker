#include <QCoreApplication>
#include <QTest>
#include <QSettings>
#include <QTime>

#include "PhTools/PhTimeCode.h"
#include "PhTools/PhDebug.h"

#include "PhTools/tests/PhTimeCodeTest.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	PhDebug::setDisplay(true, true, true, true, true);
	PhDebug::setLogMask(0b110);
	PHDEBUG << "test";
	for(int i = 0; i < 5; i++)
		PHDBG(i) << "level" << i;
	return 0;
}
