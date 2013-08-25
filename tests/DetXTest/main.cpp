/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhStrip/PhStripDoc.h"

int main(int argc, char *argv[])
{
    PhStripDoc doc;

	if(argc > 1)
		doc.openDetX(argv[1]);

	qDebug() << "Title : " << doc.getTitle();
    return 0;
}
