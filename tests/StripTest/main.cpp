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

	for(int i = 1; i < argc; i++) {
		QString fileName(argv[i]);
		PHDEBUG << fileName;
		if(!doc.openStripFile(fileName))
			continue;
		PHDEBUG << "ok";
		foreach(PhPeople *people, doc.getPeoples()) {
			int length = 0;
			foreach(PhStripText *text, doc.getTexts(people))
				length += text->getTimeOut() - text->getTimeIn();
			//PHDEBUG << people->getName() << PhTimeCode::stringFromFrame(length, doc.getTCType());
			map[people->getName()] += length;
		}
	}

	foreach(QString name, map.keys()) {
		PHDEBUG << name << ":" << PhTimeCode::stringFromFrame(map[name], PhTimeCodeType25);
	}

	return 0;
}
