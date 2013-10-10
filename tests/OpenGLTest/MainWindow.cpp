#include "MainWindow.h"
#include <QDebug>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

MainWindow::MainWindow(QWidget *parent) :
	QGLWidget(parent) {
}

void MainWindow::initializeGL()
{
	PHDEBUG << "initializeGL()";
}

void MainWindow::paintGL()
{
	PHDEBUG << "paintGL";
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainWindow::loadFile(const char *fileName)
{
	// TODO : read file, convert it in YUV and display it as a texture
}
