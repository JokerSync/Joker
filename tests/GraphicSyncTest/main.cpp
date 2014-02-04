#include "GraphicSyncTestWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QSettings settings("Phonations", "GraphicSyncTestWindow");
	int logLevel = settings.value("logLevel", 1).toInt();
	PhDebug::setLogMask(logLevel);

	QApplication a(argc, argv);
	GraphicSyncTestWindow w;
	w.show();

	return a.exec();
}
