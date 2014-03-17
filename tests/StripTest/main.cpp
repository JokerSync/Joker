#include <QDebug>

#include <QFileInfo>

#include "PhStrip/PhStripDoc.h"

using namespace std;

PhStripDoc * openDoc(QString fileName)
{
	PhStripDoc * doc = new PhStripDoc();
	if(doc->openStripFile(fileName)) {
		if(doc->getTitle().length() == 0)
			doc->setTitle(QFileInfo(fileName).baseName());
	}
	else {
		delete doc;
		doc = NULL;
	}
	return doc;
}

int computeLoopLength(PhStripDoc *doc, int loopNumber)
{
	PhFrame frameIn = PHFRAMEMIN;
	PhFrame frameOut = PHFRAMEMAX;
	if(loopNumber > 0)
		frameIn = doc->getLoops()[loopNumber - 1]->getTimeIn();
	if(loopNumber + 1 < doc->getLoops().count())
		frameOut = doc->getLoops()[loopNumber]->getTimeIn();
	return frameOut - frameIn;
}

int countLoopDetectLength(PhStripDoc *doc, int loopNumber)
{
	QMap<QString, int> map;
	/// Compute in and out
	PhFrame frameIn = PHFRAMEMIN;
	PhFrame frameOut = PHFRAMEMAX;
	if(loopNumber > 0)
		frameIn = doc->getLoops()[loopNumber - 1]->getTimeIn();
	if(loopNumber + 1 < doc->getLoops().count())
		frameOut = doc->getLoops()[loopNumber]->getTimeIn();

	int loopLength = 0;
	foreach(PhStripDetect *detect, doc->getDetects(frameIn, frameOut)) {
		int detectLength = detect->getTimeOut() - detect->getTimeIn();
		if(detectLength == 0)
			PHDEBUG << "ZEROR";
		map[detect->getPeople()->getName()] += detectLength;
		loopLength += detectLength;
		PHDBG(2) << PHNQ(PhTimeCode::stringFromFrame(detect->getTimeIn(), doc->getTCType()))
		         << PHNQ(PhTimeCode::stringFromFrame(detect->getTimeOut(), doc->getTCType()))
		         << PHNQ(PhTimeCode::stringFromFrame(loopLength, doc->getTCType()))
		         << detectLength
		         << detect->getPeople()->getName();
	}

	foreach(QString name, map.keys())
	PHDBG(1) << PHNQ(name) << ":\t" << PHNQ(PhTimeCode::stringFromFrame(map[name], doc->getTCType()));

	return loopLength;
}

int displayDetectCountPerLoop(PhStripDoc *doc)
{
	int totalLength = 0;
	PHDBG(3) << doc->getTitle();

	for(int loop = 0; loop <= doc->getLoops().count(); loop++) {
		int detectLength = countLoopDetectLength(doc, loop);
		int loopLength = computeLoopLength(doc, loop);
		//				PHDEBUG << loop << "\t:" << PHNQ(PhTimeCode::stringFromFrame(loopLength, doc->getTCType()));
		PHDBG(3) << loop << "\t:" << detectLength << "\t" << loopLength << "\t" << detectLength * 100 / loopLength;
		totalLength += detectLength;
	}

}

int countDetectLength(PhStripDoc *doc)
{
	QMap<QString, int> map;

	PHDBG(1) << PHNQ(doc->getTitle());

	int fileLength = 0;

	foreach(PhPeople *people, doc->getPeoples()) {
		int length = 0;
		foreach(PhStripDetect *detect, doc->getPeopleDetects(people)) {
			int detectLength = detect->getTimeOut() - detect->getTimeIn();
			length += detectLength;
			PHDBG(2) << PHNQ(PhTimeCode::stringFromFrame(detect->getTimeIn(), doc->getTCType()))
			         << PHNQ(PhTimeCode::stringFromFrame(detect->getTimeOut(), doc->getTCType()))
			         << PHNQ(PhTimeCode::stringFromFrame(length, doc->getTCType()))
			         << detectLength
			         << people->getName();
		}
		map[people->getName()] += length;
		fileLength += length;
	}

	PHDBG(1);

	foreach(QString name, map.keys()) {
		PHDBG(1) << PHNQ(name) << ":\t" << PHNQ(PhTimeCode::stringFromFrame(map[name], PhTimeCodeType25));
	}

	return fileLength;
}

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	PhDebug::setDisplay(false, false, false, false, false);
	//PhDebug::setLogMask(2);

	int result = 0;

	PhStripDoc doc;
	for(int i = 1; i < argc; i++) {
		if(QFile::exists(argv[i]) && !doc.openStripFile(argv[i])) {
			result = 1;
			PHDEBUG << "-------- FAILED --------";
			break;
		}
	}

	return result;
}
