#include <QDebug>

#include <QFileInfo>

#include "PhStrip/PhStripDoc.h"

using namespace std;

int main(int argc, char *argv[])
{

	// Creating a new doc:
	PhStripDoc doc;

	// This bloc is used to test with a virtual file
#if 1
	// FYI : int nbPeople, int nbLoop, int nbText, int nbTrack
	doc.createDoc(10, 3, 5, 3);
	// This bloc is used to test with a real Detx file
#else
	//Check argument count
	if (argc < 2)
	{
		PHDEBUG << "Please provide a DetX file path as argument";
		return 0;
	}

	//Check if it's a DetX file
	QFileInfo file (argv[1]);
	QString ext = file.suffix();

	if(ext != "detx")
	{
		PHDEBUG << "It's not a DetX file";
		return 0;
	}

	// Open the DetX file in argument:
	QString fileName(argv[1]);
	doc.openDetX(fileName);
#endif
	// Display the title:

	PHDEBUG << "title : " ;
	PHDEBUG <<doc.getTitle();
	PHDEBUG <<"-----------------------------";

	// Display actors

	QMap<QString, PhPeople *> list_actors = doc.getPeoples();
	PHDEBUG << "actors : ";
	QMap<QString, PhPeople *>::iterator it;

	for( it=list_actors.begin(); it!=list_actors.end() ; it++)
	{
		PHDEBUG << (*it)->getName();
	}

	PHDEBUG <<"-----------------------------";


	// Display text

	PHDEBUG << "texts : ";
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
