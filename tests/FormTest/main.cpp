#include "MainWindow.h"
#include <QApplication>
#include "PhTools/PhDebug.h"

int main(int argc, char *argv[])
{
	PhDebug::init(false, true, true, false, false, APP_NAME);
	qDebug() << APP_NAME << APP_VERSION;
	QApplication a(argc, argv);
    MainWindow window;
	window.show();

    return a.exec();
}
