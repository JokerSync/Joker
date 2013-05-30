#include "SDL/SDL.h"

#include "TextTestWindow.h"

int main( int argc, char* argv[] )
{
    if (argc < 2){
        qDebug() << "Please specify a file";
        return -1;
    }
    QApplication a(argc, argv);
    TextTestWindow w;
    w.show();


    return a.exec();
}

