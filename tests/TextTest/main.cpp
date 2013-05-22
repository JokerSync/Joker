#include "TextTestWindow.h"
#include "SDL/SDL.h"

int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);
    TextTestWindow w;
    w.show();

    return a.exec();
}

