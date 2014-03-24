#include <QDebug>

#include <QFileInfo>

#include "PhStrip/PhStripDoc.h"

using namespace std;

PhStripDoc * openDoc(QString fileName)
{
	PhStripDoc * doc = new PhStripDoc();
	if(doc->openStripFile(fileName)) {
		if(doc->title().length() == 0)
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
		frameIn = doc->loops()[loopNumber - 1]->frameIn();
	if(loopNumber + 1 < doc->loops().count())
		frameOut = doc->loops()[loopNumber]->frameIn();
	return frameOut - frameIn;
}

int countLoopDetectLength(PhStripDoc *doc, int loopNumber)
{
	QMap<QString, int> map;
	/// Compute in and out
	PhFrame frameIn = PHFRAMEMIN;
	PhFrame frameOut = PHFRAMEMAX;
	if(loopNumber > 0)
		frameIn = doc->loops()[loopNumber - 1]->frameIn();
	if(loopNumber + 1 < doc->loops().count())
		frameOut = doc->loops()[loopNumber]->frameIn();

	int loopLength = 0;
	foreach(PhStripDetect *detect, doc->detects(frameIn, frameOut)) {
		int detectLength = detect->frameOut() - detect->frameIn();
		if(detectLength == 0)
			PHDEBUG << "ZEROR";
		map[detect->people()->name()] += detectLength;
		loopLength += detectLength;
		PHDBG(2) << PHNQ(PhTimeCode::stringFromFrame(detect->frameIn(), doc->timeCodeType()))
		         << PHNQ(PhTimeCode::stringFromFrame(detect->frameOut(), doc->timeCodeType()))
		         << PHNQ(PhTimeCode::stringFromFrame(loopLength, doc->timeCodeType()))
		         << detectLength
		         << detect->people()->name();
	}

	foreach(QString name, map.keys())
	PHDBG(1) << PHNQ(name) << ":\t" << PHNQ(PhTimeCode::stringFromFrame(map[name], doc->timeCodeType()));

	return loopLength;
}

int displayDetectCountPerLoop(PhStripDoc *doc)
{
	int totalLength = 0;
	PHDBG(3) << doc->title();

	for(int loop = 0; loop <= doc->loops().count(); loop++) {
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

	PHDBG(1) << PHNQ(doc->title());

	int fileLength = 0;

	foreach(PhPeople *people, doc->peoples()) {
		int length = 0;
		foreach(PhStripDetect *detect, doc->peopleDetects(people)) {
			int detectLength = detect->frameOut() - detect->frameIn();
			length += detectLength;
			PHDBG(2) << PHNQ(PhTimeCode::stringFromFrame(detect->frameIn(), doc->timeCodeType()))
			         << PHNQ(PhTimeCode::stringFromFrame(detect->frameOut(), doc->timeCodeType()))
			         << PHNQ(PhTimeCode::stringFromFrame(length, doc->timeCodeType()))
			         << detectLength
			         << people->name();
		}
		map[people->name()] += length;
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
