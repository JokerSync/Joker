#include <QtWidgets/QApplication>
#include <QSettings>

#include "MainView.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QSettings settings("Phonations", "VideoTest");

	PhDebug::init(false, true, true, true, true, true, 1, "VideoTest");
	MainView mainView(&settings);
	mainView.resize(800, 600);

    mainView.show();

	QString fileName = settings.value("lastVideoFile", "").toString();
	if(argc > 1)
		fileName = argv[1];
	if(QFile(fileName).exists())
		mainView.openFile(QString(fileName));

    return app.exec();
}
