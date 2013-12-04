#include "GraphicSyncTestWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GraphicSyncTestWindow w;
	w.show();

	return a.exec();
}
