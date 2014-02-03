#include "GraphicSyncTestWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
<<<<<<< HEAD

	QSettings settings("Phonations", "GraphicSyncTestWindow");
	int logLevel = settings.value("logLevel", 1).toInt();
	PhDebug::setLogMask(logLevel);

=======
>>>>>>> origin/master
	QApplication a(argc, argv);
	GraphicSyncTestWindow w;
	w.show();

	return a.exec();
}
