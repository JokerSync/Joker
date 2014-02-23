#include <QtWidgets/QApplication>
#include <QSettings>
#include <QFile>

#include "VideoTestWindow.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QSettings settings("Phonations", "VideoTest");

	VideoTestWindow w(&settings);
	w.resize(800, 600);

	w.show();

	QString fileName = settings.value("lastVideoFile", "").toString();
	if(argc > 1)
		fileName = argv[1];
	if(QFile(fileName).exists())
		w.openFile(QString(fileName));

	return app.exec();
}
