#include "LTCToolWindow.h"
#include <QApplication>

#include <QSettings>

int main(int argc, char *argv[])
{
	QSettings settings("Phonations", APP_NAME);
	QSettings::setDefaultFormat(QSettings::NativeFormat);

	QApplication a(argc, argv);
	LTCToolWindow w(&settings);
	w.show();

	return a.exec();
}
