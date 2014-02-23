#include "OpenGLTestWindow.h"
#include <QDebug>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

OpenGLTestWindow::OpenGLTestWindow(QWidget *parent) :
	QGLWidget(parent) {
}

void OpenGLTestWindow::initializeGL()
{
	qDebug() << "initializeGL()";
}

void OpenGLTestWindow::paintGL()
{
	qDebug() << "paintGL";
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLTestWindow::loadFile(const char *fileName)
{
	// TODO : read file, convert it in YUV and display it as a texture
}
