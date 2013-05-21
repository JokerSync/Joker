#include "PhGraphic/phglwidget.h"
#include "videotest.h"

videoTest::videoTest(QWidget *parent)
    : PhGLWidget( parent, "displaying video thanks to libvlc")
{
}


void videoTest::initializeGL()
{
    glClearColor(.90,.90,.90,0); 	//Background color RGBA
}

void videoTest::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	// Clear the  framebuffer & the depthbuffer
}
