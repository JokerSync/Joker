#include "GraphicSyncTestWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	PhDebug::init(false, true, true, true, true, false, "GraphicSyncTest");

	QApplication a(argc, argv);
	GraphicSyncTestWindow w;
	w.show();

	return a.exec();
}
