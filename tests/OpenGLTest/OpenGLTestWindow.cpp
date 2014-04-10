#include "OpenGLTestWindow.h"
#include <QDebug>

OpenGLTestWindow::OpenGLTestWindow(QWidget *parent) :
	QGLWidget(parent) {
}

void OpenGLTestWindow::initializeGL()
{
	qDebug() << "initializeGL()";

	connect(&_refreshTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
	_refreshTimer.setInterval(0);
	_refreshTimer.start();
	_timer.start();
}

void OpenGLTestWindow::paintGL()
{
	qDebug() << "paintGL:" << _timer.elapsed();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_timer.restart();
}
