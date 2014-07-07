#include "SonyTestWindow.h"
#include <QApplication>

#include "PhTools/PhDebug.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
