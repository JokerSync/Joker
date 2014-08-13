#include <QApplication>

#include "PhTools/PhDebug.h"

#include "GraphicSyncTestWindow.h"

int main(int argc, char *argv[])
{
	GraphicSyncTestSettings settings;
	PhDebug::setLogMask(settings.logLevel());

	QApplication a(argc, argv);
	GraphicSyncTestWindow w(&settings);
	w.show();

	return a.exec();
}
