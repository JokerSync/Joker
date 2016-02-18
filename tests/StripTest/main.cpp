#include <QDebug>

#include <QFileInfo>

#include "PhTools/PhDebug.h"
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
		PHDBG(2) << PHNQ(PhTimeCode::stringFromTime(detect->timeIn(), PhTimeCodeType25))
		         << PHNQ(PhTimeCode::stringFromTime(detect->timeOut(), PhTimeCodeType25))
		         << PHNQ(PhTimeCode::stringFromTime(loopLength, PhTimeCodeType25))
		         << detectLength
		         << detect->people()->name();
	}

	foreach(QString name, map.keys())
		PHDBG(1) << PHNQ(name) << ":\t" << PHNQ(PhTimeCode::stringFromTime(map[name], PhTimeCodeType25));

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
			PHDBG(2) << PHNQ(PhTimeCode::stringFromTime(detect->timeIn(), PhTimeCodeType25))
			         << PHNQ(PhTimeCode::stringFromTime(detect->timeOut(), PhTimeCodeType25))
			         << PHNQ(PhTimeCode::stringFromTime(length, PhTimeCodeType25))
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
	PHDEBUG << "Title:" << doc->title();
	PHDEBUG << "Video:" << doc->videoFilePath();
	PHDEBUG << "People count:" << doc->peoples().count();
	foreach(PhPeople *people, doc->peoples()) {
		PHDEBUG << people->name();
	}

	PHDEBUG << "Text count:" << doc->texts().count();
	foreach(PhStripText *text, doc->texts()) {
		QString name = "???";
		if(text->people())
			name = text->people()->name();
		PHDEBUG << text->y() << "-"
		        << name << ":"
		        << PhTimeCode::stringFromTime(text->timeIn(), PhTimeCodeType25)
		        << " -> "
		        << PhTimeCode::stringFromTime(text->timeOut(), PhTimeCodeType25)
		        << text->content();
	}

	foreach(PhStripText *text, doc->texts(true)) {
		QString name = "???";
		if(text->people())
			name = text->people()->name();
		PHDEBUG << text->y() <<"-"
		        << name << ":"
		        << PhTimeCode::stringFromTime(text->timeIn(), PhTimeCodeType25)
		        << " -> "
		        << PhTimeCode::stringFromTime(text->timeOut(), PhTimeCodeType25)
		        << text->content();
	}

	foreach(PhStripDetect *detect, doc->detects()) {
		QString name = "???";
		if(detect->people())
			name = detect->people()->name();
		PHDEBUG << detect->y()<< "-"
		        << name << ":"
		        << PhTimeCode::stringFromTime(detect->timeIn(), PhTimeCodeType25)
		        << " -> "
		        << PhTimeCode::stringFromTime(detect->timeOut(), PhTimeCodeType25)
		        << "type:" << detect->type();
	}
}

bool generate(QString fileName) {
	PHDEBUG << fileName;
	PhStripDoc doc;

	doc.setTitle("The man with the golden arms");
	for(int i = 0; i < 200; i++) {
		doc.addLoop(new PhStripLoop(i * 960 * 25 * 60, QString::number(i)));
	}

	return doc.exportDetXFile(fileName, PhTimeCode::timeFromString("01:00:00:00", PhTimeCodeType25));
}

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	PHDEBUG << "StripTest";
	PhStripDoc doc;

//	PHDEBUG << doc.computeDetXId("abc");
//	PHDEBUG << doc.computeDetXId("ABC");
//	PHDEBUG << doc.computeDetXId("It's ok!");

//	PHDEBUG << doc.importDetXFile("/Users/martin/dev/builds/StripSpec-Desktop_Qt_5_3_clang_64bit-Debug/save01.detx");
//	PHDEBUG << doc.title();

//	return 0;

//	StripTestSettings settings;
//	PhDebug::setDisplay(false, false, false, false, false);
	//PhDebug::setLogMask(settings.logMask());

	int result = 0;

//	for(int i = 1; i < argc; i++) {
//		QString fileName = argv[i];
//		if(QFile::exists(fileName)) {
//			if(doc.openStripFile(fileName)) {
//				displayDoc(&doc);
//			}
//			else {
//				result = 1;
//				PHDEBUG << "-------- FAILED --------";
//				break;
//			}
//		}
//		else
//			PHDEBUG << fileName << "doesn't exists!!!";
//	}
	PHDEBUG << argc;
	if(argc > 1) {
		if(!generate(argv[1]))
			result = 1;
	}

	return result;
}
