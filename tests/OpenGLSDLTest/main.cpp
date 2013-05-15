
#include "TestSDLOpenGLWidget.h"

#include "SDL/SDL.h"

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);
    TestSDLOpenGLWidget w ;
    w.show();
    return a.exec();

}
