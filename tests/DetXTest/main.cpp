/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhStrip/PhStripDoc.h"

int main(int argc, char *argv[])
{
    PhStripDoc doc;

	if(argc > 1) {
		QString fileName = argv[1];
		qDebug() << fileName;
		doc.openDetX(fileName);
	}
	qDebug() << "Title : " << doc.getTitle();
	qDebug() << "People count : " << doc.getPeoples().count();
	foreach(QString peopleName, doc.getPeoples().keys())
	{
		qDebug() << doc.getPeoples()[peopleName]->getName();
	}

	foreach(PhStripText * text, doc.getTexts())
	{
		qDebug() << text->getPeople().getName() << " : " << text->getContent();
	}

    return 0;
}
