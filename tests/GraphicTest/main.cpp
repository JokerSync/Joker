
#include "GraphicTestWindow.h"

#include <stdio.h>
#include <stdlib.h>

#include <gl.h>
#include <glu.h>




int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);

    GraphicTestWindow w ;
    w.show();
    return a.exec();

}
