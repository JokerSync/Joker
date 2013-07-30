

#include <QtWidgets/QApplication>
#include "MainView.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainView mainView;
    mainView.resize(320, 240);
    mainView.show();

    return app.exec();
}
