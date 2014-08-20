#include <QApplication>

#include "PhTools/PhDebug.h"

#include "SonyToolWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SonyToolWindow w;
	w.show();

	return a.exec();
}
