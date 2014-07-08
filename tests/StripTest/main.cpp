#include <QDebug>

#include <QFileInfo>

#include "PhStrip/PhStripDoc.h"

#include "StripTestSettings.h"

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
	PhTime timeIn = PHTIMEMIN;
	PhTime timeOut = PHTIMEMAX;
	if(loopNumber > 0)
		timeIn = doc->loops()[loopNumber - 1]->timeIn();
	if(loopNumber + 1 < doc->loops().count())
		timeOut = doc->loops()[loopNumber]->timeIn();
	return timeOut - timeIn;
}

int countLoopDetectLength(PhStripDoc *doc, int loopNumber)
{
	QMap<QString, int> map;
	/// Compute in and out
	PhTime timeIn = PHTIMEMIN;
	PhTime timeOut = PHTIMEMAX;
	if(loopNumber > 0)
		timeIn = doc->loops()[loopNumber - 1]->timeIn();
	if(loopNumber + 1 < doc->loops().count())
		timeOut = doc->loops()[loopNumber]->timeIn();

	int loopLength = 0;
	foreach(PhStripDetect *detect, doc->detects(timeIn, timeOut)) {
		int detectLength = detect->timeOut() - detect->timeIn();
		if(detectLength == 0)
			PHDEBUG << "ZEROR";
		map[detect->people()->name()] += detectLength;
		loopLength += detectLength;
		PHDBG(2) << PHNQ(PhTimeCode::stringFromTime(detect->timeIn(), doc->timeCodeType()))
		         << PHNQ(PhTimeCode::stringFromTime(detect->timeOut(), doc->timeCodeType()))
		         << PHNQ(PhTimeCode::stringFromTime(loopLength, doc->timeCodeType()))
		         << detectLength
		         << detect->people()->name();
	}

	foreach(QString name, map.keys())
	PHDBG(1) << PHNQ(name) << ":\t" << PHNQ(PhTimeCode::stringFromTime(map[name], doc->timeCodeType()));

	return loopLength;
}

int displayDetectCountPerLoop(PhStripDoc *doc)
{
	int totalLength = 0;
	PHDBG(3) << doc->title();

	for(int loop = 0; loop <= doc->loops().count(); loop++) {
		int detectLength = countLoopDetectLength(doc, loop);
		int loopLength = computeLoopLength(doc, loop);
		//				PHDEBUG << loop << "\t:" << PHNQ(PhTimeCode::stringFromTime(loopLength, doc->getTCType()));
		PHDBG(3) << loop << "\t:" << detectLength << "\t" << loopLength << "\t" << detectLength * 100 / loopLength;
		totalLength += detectLength;
	}
	return totalLength;
}

int countDetectLength(PhStripDoc *doc)
{
	QMap<QString, int> map;

	PHDBG(1) << PHNQ(doc->title());

	int fileLength = 0;

	foreach(PhPeople *people, doc->peoples()) {
		int length = 0;
		foreach(PhStripDetect *detect, doc->peopleDetects(people)) {
			int detectLength = detect->timeOut() - detect->timeIn();
			length += detectLength;
			PHDBG(2) << PHNQ(PhTimeCode::stringFromTime(detect->timeIn(), doc->timeCodeType()))
			         << PHNQ(PhTimeCode::stringFromTime(detect->timeOut(), doc->timeCodeType()))
			         << PHNQ(PhTimeCode::stringFromTime(length, doc->timeCodeType()))
			         << detectLength
			         << people->name();
		}
		map[people->name()] += length;
		fileLength += length;
	}

	PHDBG(1);

	foreach(QString name, map.keys()) {
		PHDBG(1) << PHNQ(name) << ":\t" << PHNQ(PhTimeCode::stringFromTime(map[name], PhTimeCodeType25));
	}

	return fileLength;
}

void displayDoc(PhStripDoc* doc)
{
	PHDEBUG << doc->title();
	foreach(PhPeople *people, doc->peoples()) {
		PHDEBUG << people->name();
	}

	foreach(PhStripText *text, doc->texts()) {
		QString name = "???";
		if(text->people())
			name = text->people()->name();
		PHDEBUG << text->y() << "-"
		        << name << ":"
		        << PhTimeCode::stringFromTime(text->timeIn(), doc->timeCodeType())
		        << " -> "
		        << PhTimeCode::stringFromTime(text->timeOut(), doc->timeCodeType())
		        << text->content();
	}

	foreach(PhStripText *text, doc->texts(true)) {
		QString name = "???";
		if(text->people())
			name = text->people()->name();
		PHDEBUG << text->y() <<"-"
		        << name << ":"
		        << PhTimeCode::stringFromTime(text->timeIn(), doc->timeCodeType())
		        << " -> "
		        << PhTimeCode::stringFromTime(text->timeOut(), doc->timeCodeType())
		        << text->content();
	}

	foreach(PhStripDetect *detect, doc->detects()) {
		QString name = "???";
		if(detect->people())
			name = detect->people()->name();
		PHDEBUG << detect->y()<< "-"
		        << name << ":"
		        << PhTimeCode::stringFromTime(detect->timeIn(), doc->timeCodeType())
		        << " -> "
		        << PhTimeCode::stringFromTime(detect->timeOut(), doc->timeCodeType())
		        << "type:" << detect->type();
	}
}

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	StripTestSettings settings;
	PhDebug::setDisplay(false, false, false, false, false);
	PhDebug::setLogMask(settings.logMask());

	int result = 0;

	PhStripDoc doc;
	for(int i = 1; i < argc; i++) {
		QString fileName = QLatin1String(argv[i]);
		if(QFile::exists(fileName)) {
			if(doc.openStripFile(fileName)) {
				displayDoc(&doc);
			}
			else {
				result = 1;
				PHDEBUG << "-------- FAILED --------";
				break;
			}
		}
	}

	return result;
}
