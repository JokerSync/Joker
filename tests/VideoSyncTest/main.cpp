#include <QtWidgets/QApplication>
#include "MainView.h"
#include <QSettings>
#include <QFile>

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QSettings settings("Phonations", "VideoSyncTest");
	int logLevel = settings.value("logLevel", 1).toInt();

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
