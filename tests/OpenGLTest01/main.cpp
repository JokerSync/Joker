
#include "myglwidget.h"
#include "mywindow.h"
#include "SDL/SDL.h"

#include <stdio.h>
#include <stdlib.h>

#include <gl.h>
#include <glu.h>




int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);

    myWindow w ;
    w.show();
    return a.exec();

}
