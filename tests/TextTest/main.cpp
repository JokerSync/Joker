#include "SDL/SDL.h"

#include "TextTestWindow.h"

int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);
    TextTestWindow w;
    w.show();


    return a.exec();
}

