
#include "testphglwidget.h"

#include <stdio.h>
#include <stdlib.h>

#include <gl.h>
#include <glu.h>




int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);

    TestPhGLWidget w ;
    w.show();
    return a.exec();

}
