#include <QDebug>

#include <QFileInfo>

#include "PhStrip/PhStripDoc.h"

using namespace std;

int main(int argc, char *argv[])
{
	// Creating a new doc:
	PhStripDoc doc;

	//Check argument count
	if (argc > 1)
	{
		// Open the DetX file in argument:
		QString fileName(argv[1]);
		doc.openDetX(fileName);
	}
	else
		doc.createDoc("test", 3, 3, 9, 3, 0);

	// Display the title:
	PHDEBUG << "title : " << doc.getTitle();
	PHDEBUG << "video file : " << doc.getVideoPath();
	PHDEBUG << "video timestamp" << PhTimeCode::stringFromFrame(doc.getVideoTimestamp(), doc.getTCType());

	// Display actors:
	QMap<QString, PhPeople *> list_actors = doc.getPeoples();
	QMap<QString, PhPeople *>::iterator people;
	PHDEBUG <<"--------- actors ---------";
	for( people=list_actors.begin(); people!=list_actors.end() ; people++)
	{
		PHDEBUG << (*people)->getName();
	}

	// Display text

	PHDEBUG <<"--------- texts ---------";
	QString line;
	PhPeople * lastPeople;

	foreach(PhStripText* text, doc.getTexts())
	{
		line = text->getPeople()->getName() + " : " + text->getContent();
		PHDEBUG << line;

		//             if(((*text)->getPeople().getName()) != ((*(text-1))->getPeople().getName()))
		//             {
		//                 PHDEBUG << qPrintable( line );
		//                 line = (*text)->getPeople().getName();
		//                 line += " : ";
		//                 line += (*text)->getContent();
		//             }
		//             else
		//             {
		//                 line += (*text)->getContent();
		//             }
	}

	foreach (PhStripOff * off, doc.getOffs()) {
		PHDEBUG << off->getPeople()->getName() << " : " << PhTimeCode::stringFromFrame(off->getTimeIn(), doc.getTCType())
				 << " - " << PhTimeCode::stringFromFrame(off->getTimeOut(), doc.getTCType());
	}

	return 0;
}
