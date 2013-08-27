#include <QtWidgets/QApplication>
#include "MainView.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainView mainView;
	mainView.resize(640, 600);

    if(argc>1)
        mainView.openFile(QString(argv[1]));

    mainView.show();

    return app.exec();
}
