#include <qmath.h>
#include <QCoreApplication>

#include "PhTools/PhDebug.h"
#include "GraphicTestView.h"
#include "PhTools/PhPictureTools.h"

GraphicTestView::GraphicTestView(QWidget *parent)
	: PhGraphicView( parent),
	_text1(&_font1, "eéaàiîoô"),
	_text2(&_font2, "The party is over!")
{
}

bool GraphicTestView::setFontFile(QString fontFile)
{
	return _font1.setFontFile(fontFile);
}

bool GraphicTestView::init()
{
	PHDEBUG << "Initialize _image";

	QString imageFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240_black.png";
	_image.setFilename(imageFile);
	_image.setTextureCoordinate(1, 1);
	_image.setPosition(50, 0, 1);
	if (!_image.init()) {
		PHDEBUG << "_image not initialize : " << imageFile;
		return false;
	}
	_image.setSize(_image.originalSize());

	PHDEBUG << "Initialize _font";
	if (!_font1.setFontFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/SWENSON.TTF")) {
		PHDEBUG << "SWENSON not initialize using path : " << QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/SWENSON.TTF";
		return false;
	}

	if (!_font2.setFontFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/Arial.ttf")) {
		PHDEBUG << "ARIAL not initialize";
		return false;
	}

	PHDEBUG << "Initialize _text";
	_text1.setRect(50, 100, 100, 100);
	_text1.setColor(QColor(255, 0, 0));
	_text1.setZ(-1);

	_text2.setRect(50, 300, 500, 100);
	_text2.setColor(QColor(255, 0, 0));
	_text2.setZ(-1);

	PHDEBUG << "Initialize _rect";
	_rect.setRect(100, 100, 75, 40);
	_rect.setColor(QColor(0, 255, 0));
	_rect.setZ(-2);

	PHDEBUG << "Initialize _loop";
	_loop.setX(100);
	_loop.setY(50);
	_loop.setZ(-3);
	_loop.setWidth(120);
	_loop.setHeight(100);
	_loop.setHThick(5);
	_loop.setCrossHeight(60);
	_loop.setColor(QColor(1, 255, 1));
	if (!_loop.init()) {
		PHDEBUG << "_loop not initialize";
		return false;
	}

	int w = 50;
	int h = 50;
	unsigned char * yuv = PhPictureTools::generateYUVPattern(w, h);
	_yuvRect.setRect(20, 300, 150, 100);
	_yuvRect.createTextureFromYUVBuffer(yuv, w, h);

	_disc.setPosition(300, 300, 0);
	_disc.setRadius(100);
	_disc.setColor(Qt::yellow);
	return true;
}

void GraphicTestView::paint()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//	_image.setTextureCoordinate(1, 3);
	_image.draw();

	_rect.setColor(QColor(0, 255, 0));
	_rect.setRect(50, 100, 500, 75);
	_rect.draw();

	_rect.setColor(QColor(0, 255, 255));
	_rect.setRect(50, 175, 500, 25);
	_rect.draw();

	_text1.setRect(50, 100, 500, 100);
	_text1.draw();

//	_text2.draw();



	_font1.select();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor3f(0, 0, 1);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


	glBegin(GL_QUADS);  //Begining the cube's drawing
	{
		glTexCoord3f(0, 0, 1);  glVertex3f(0,               0,              0);
		glTexCoord3f(1, 0, 1);  glVertex3f(this->width(),   0,              0);
		glTexCoord3f(1, 1, 1);  glVertex3f(this->width(),   this->height(), 0);
		glTexCoord3f(0, 1, 1);  glVertex3f(0,               this->height(), 0);
	}
	glEnd();

//	_text.setX(_text.getX() + 4);
//	if(_text.getX() > this.width())
//		_text.setX(0);
//	if((_text.getX()+_text.getWidth()) < 0)
//		_text.setX(this.width());


//	_loop.draw();

	//_yuvRect.draw();

	PhGraphicText frameRateText(&_font2, QString::number(this->refreshRate()));
	frameRateText.setRect(0, 100, 100, 100);
	frameRateText.setColor(Qt::red);
	frameRateText.draw();

	_disc.draw();

}




