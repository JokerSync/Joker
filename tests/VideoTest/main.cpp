#include <QtWidgets/QApplication>
#include "MainView.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PhDebug::init(false, true, true, true, true);
    MainView mainView;
	mainView.resize(800, 600);

    if(argc>1)
        mainView.openFile(QString(argv[1]));

    mainView.show();

    return app.exec();
}
