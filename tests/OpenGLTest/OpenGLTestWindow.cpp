#include <QDebug>
#include <QApplication>

#include "OpenGLTestWindow.h"

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

	glEnable( GL_TEXTURE_2D );

	// create first texture
	QImage image1(QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "interlace_008.bmp");
	qDebug() << image1.size() << image1.format();

	glGenTextures( 1, &_texture1 );

	glBindTexture( GL_TEXTURE_2D, _texture1 );

	glEnable( GL_TEXTURE_2D );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image1.width(), image1.height(), 0,
				  GL_BGRA, GL_UNSIGNED_BYTE, image1.bits());

	int filterSetting = GL_LINEAR;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterSetting);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterSetting);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glDisable(GL_TEXTURE_2D);

	// create second texture
	QImage image2(QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "interlace_100.bmp");
	qDebug() << image2.size() << image2.format();

	glGenTextures( 1, &_texture2 );

	glBindTexture( GL_TEXTURE_2D, _texture2 );

	glEnable( GL_TEXTURE_2D );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image2.width(), image2.height(), 0,
				  GL_BGRA, GL_UNSIGNED_BYTE, image2.bits());

	glDisable(GL_TEXTURE_2D);

	filterSetting = GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterSetting);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterSetting);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void OpenGLTestWindow::paintGL()
{
	//qDebug() << "paintGL:" << _timer.elapsed();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_timer.restart();

	glBindTexture( GL_TEXTURE_2D, _texture1 );

	glEnable(GL_TEXTURE_2D);

	//        (0,0) ------ (1,0)
	//          |            |
	//          |            |
	//        (0,1) ------ (1,1)

	float _tu = 1.0f;
	float _tv = 1.0f;
	int x = 0;
	int y = 0;
	int z = 0;
	int width = this->width();
	int height = this->height();

	glBegin(GL_QUADS);  //Begining the cube's drawing
	{
		glTexCoord3f(0, 0, 1);      glVertex3i(x,      y, z);
		glTexCoord3f(_tu, 0, 1);    glVertex3i(x + width, y, z);
		glTexCoord3f(_tu, _tv, 1);  glVertex3i(x + width, y + height,  z);
		glTexCoord3f(0, _tv, 1);    glVertex3i(x,      y + height,  z);
	}
	glEnd();

	x += width;

	glBindTexture( GL_TEXTURE_2D, _texture2 );

	glBegin(GL_QUADS);  //Begining the cube's drawing
	{
		glTexCoord3f(0, 0, 1);      glVertex3i(x,      y, z);
		glTexCoord3f(_tu, 0, 1);    glVertex3i(x + width, y, z);
		glTexCoord3f(_tu, _tv, 1);  glVertex3i(x + width, y + height,  z);
		glTexCoord3f(0, _tv, 1);    glVertex3i(x,      y + height,  z);
	}
	glEnd();


	glDisable(GL_TEXTURE_2D);
}

void OpenGLTestWindow::resizeGL(int width, int height)
{
	if(height == 0)
		height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLoadIdentity();
}
