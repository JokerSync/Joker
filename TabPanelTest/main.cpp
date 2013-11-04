/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PrefPanel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PrefPanel w;
	w.show();

	return a.exec();
}
