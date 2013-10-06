#include "MainWindow.h"
#include <QApplication>
#include "PhTools/PhDebug.h"

int main(int argc, char *argv[])
{
	PhDebug::init(false, false, true, false, false);
    QApplication a(argc, argv);
    MainWindow window;
	window.show();

    return a.exec();
}
