#include <QtWidgets/QApplication>
#include "MainView.h"
#include <QSettings>

int main(int argc, char *argv[])
{
	QSettings settings("Phonations", "VideoSyncTest");
	int logLevel = settings.value("logLevel", 1).toInt();
	PhDebug::init(false, true, true, true, true, true, logLevel, "VideoSyncTest");

	QApplication app(argc, argv);

	MainView mainView(&settings);
	mainView.resize(800, 600);

	mainView.show();

	QString fileName = "";
	if (argc > 1)
		fileName = argv[1];
	else
		fileName = settings.value("lastFile").toString();

	if(QFile(fileName).exists())
		mainView.openFile(fileName);

	return app.exec();
}
