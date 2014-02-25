#include <QDebug>

#include <QFileInfo>

#include "PhStrip/PhStripDoc.h"
#include "PhStrip/tests/PhStripDocTest.h"

using namespace std;

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	//	PhStripDocTest docTest;
	//	int test = QTest::qExec(&docTest);
	//	if(test)
	//		return test;

	// Creating a new doc:
	PhStripDoc doc;

	QMap<QString, int> map;

	PhDebug::setDisplay(false, false, false, false, false);
	PhDebug::setLogMask(2);

	int totalLength = 0;
	for(int i = 1; i < argc; i++) {
		QString fileName(argv[i]);
		PHDBG(1) << PHNQ(fileName);
		if(!doc.openStripFile(fileName))
			continue;
		int fileLength = 0;
		foreach(PhPeople *people, doc.getPeoples()) {
			int length = 0;
			foreach(PhStripDetect *detect, doc.getDetects(people)) {
				int detectLength = detect->getTimeOut() - detect->getTimeIn();
				length += detectLength;
				PHDBG(2) << PHNQ(PhTimeCode::stringFromFrame(detect->getTimeIn(), doc.getTCType()))
						<< PHNQ(PhTimeCode::stringFromFrame(detect->getTimeOut(), doc.getTCType()))
						<< PHNQ(PhTimeCode::stringFromFrame(length, doc.getTCType()))
						<< detectLength
						<< people->getName();
			}
			map[people->getName()] += length;
			fileLength += length;
		}
//		PHDBG(1) << PHNQ(fileName) << ":\t" << PHNQ(PhTimeCode::stringFromFrame(fileLength, PhTimeCodeType25));
		totalLength += fileLength;
	}

	PHDBG(1);

	foreach(QString name, map.keys()) {
		PHDBG(1) << PHNQ(name) << ":\t" << PHNQ(PhTimeCode::stringFromFrame(map[name], PhTimeCodeType25));
	}

	PHDBG(1);

	PHDBG(1) << "TOTAL :\t" << PHNQ(PhTimeCode::stringFromFrame(totalLength, PhTimeCodeType25));

	return 0;
}
