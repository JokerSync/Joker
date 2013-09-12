#include <QDebug>

#include <QFileInfo>

#include "PhStrip/PhStripDoc.h"

using namespace std;

int main(int argc, char *argv[])
{
    //Check argument count
    if (argc < 2)
    {
        qDebug() << "Please provide a DetX file path as argument";
        return 0;
    }

    //Check if it's a DetX file
    QFileInfo file (argv[1]);
    QString ext = file.suffix();

    if(ext != "detx")
    {
        qDebug() << "It's not a DetX file";
        return 0;
    }

    // Creating a new doc:
    PhStripDoc doc;

    // Open the DetX file in argument:
	QString fileName(argv[1]);
    doc.openDetX(fileName);

    // Display the title:

    qDebug() << "title : " ;
    qDebug() <<doc.getTitle();
    qDebug() <<"-----------------------------";

    // Display actors

    QMap<QString, PhPeople *> list_actors = doc.getPeoples();
    qDebug() << "actors : ";
    QMap<QString, PhPeople *>::iterator it;

    for( it=list_actors.begin(); it!=list_actors.end() ; it++)
    {
        qDebug() << (*it)->getName();
    }

     qDebug() <<"-----------------------------";


    // Display text

    qDebug() << "texts : ";
	QString line;
	PhPeople * lastPeople;

	foreach(PhStripText* text, doc.getTexts())
    {
		line = text->getPeople()->getName() + " : " + text->getContent();
		qDebug() << line;

//             if(((*text)->getPeople().getName()) != ((*(text-1))->getPeople().getName()))
//             {
//                 qDebug() << qPrintable( line );
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
		qDebug() << off->getPeople()->getName() << " : " << PhTimeCode::stringFromFrame(off->getTimeIn(), doc.getTCType())
				 << " - " << PhTimeCode::stringFromFrame(off->getTimeOut(), doc.getTCType());
	}

     qDebug() << qPrintable( line );

    return 0;
}
