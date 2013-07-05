

#include <QtWidgets/QApplication>
#include "PhVideo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PhVideo player;
    player.resize(320, 240);
    player.show();

    return app.exec();
}
