#include <QtWidgets/QApplication>
#include <QSettings>

#include "AVTestWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QSettings settings(PH_ORG_NAME, PH_APP_NAME);

	AVTestWindow w(&settings);
	w.resize(800, 600);

	w.show();

	QString fileName = settings.value("lastVideoFile", "").toString();
	if(argc > 1)
		fileName = argv[1];
	if(QFile(fileName).exists())
		w.openFile(QString(fileName));

	return app.exec();
}
