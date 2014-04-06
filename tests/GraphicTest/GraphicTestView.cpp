#include <qmath.h>
#include <QCoreApplication>

#include "PhTools/PhDebug.h"
#include "GraphicTestView.h"
#include "PhTools/PhPictureTools.h"
#include "PhGraphic/PhGraphicDashedLine.h"
#include "PhGraphic/PhGraphicArrow.h"

GraphicTestView::GraphicTestView(QWidget *parent)
	: PhGraphicView( parent)
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

	PHDEBUG << "Initialize _rect";
	_rect.setRect(100, 100, 75, 40);
	_rect.setColor(QColor(0, 255, 0));
	_rect.setZ(-2);

	int w = 50;
	int h = 50;
	unsigned char * yuv = PhPictureTools::generateYUVPattern(w, h);
	_yuvRect.setRect(20, 300, 150, 100);
	_yuvRect.createTextureFromYUVBuffer(yuv, w, h);

	return true;
}

void GraphicTestView::paint()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	_image.setTextureCoordinate(1, 3);
	_image.draw();

	_rect.setColor(QColor(0, 255, 0));
	_rect.setRect(50, 100, 500, 75);
	_rect.draw();

	_rect.setColor(QColor(0, 255, 255));
	_rect.setRect(50, 175, 500, 25);
	_rect.draw();

	PhGraphicText text1(&_font1, "eéaàiîoô");

	text1.setRect(50, 100, 100, 100);
	text1.setColor(QColor(255, 0, 0));
	text1.setZ(-1);
	text1.setRect(50, 100, 500, 100);
	text1.draw();

	PhGraphicText text2(&_font2, "The party is over!");
	text2.setRect(50, 300, 500, 100);
	text2.setColor(QColor(255, 0, 0));
	text2.setZ(-1);
	text2.draw();

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

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

//	_text.setX(_text.getX() + 4);
//	if(_text.getX() > this.width())
//		_text.setX(0);
//	if((_text.getX()+_text.getWidth()) < 0)
//		_text.setX(this.width());


	PhGraphicLoop loop;
	loop.setPosition(100, 50, -3);
	loop.setSize(120, 100);
	loop.setHThick(5);
	loop.setCrossHeight(60);
	loop.setColor(QColor(1, 255, 1));
	loop.draw();

	//_yuvRect.draw();

	PhGraphicDisc disc(300, 300, 100);
	disc.setColor(Qt::yellow);
	disc.draw();

	for (int i = 0; i < 5; ++i) {
		PhGraphicDashedLine line(i, 0, 50*i, 300, 30);
		line.setColor(Qt::green);
		line.setZ(4);
		line.draw();
	}

	PhGraphicArrow arrow1(PhGraphicArrow::DownLeftToUpRight, 150, 250, 200, 100);
	arrow1.setColor(Qt::red);
	arrow1.setZ(5);
	arrow1.draw();

	PhGraphicArrow arrow2(PhGraphicArrow::UpLefToDownRight, 150, 400, 200, 100);
	arrow2.setColor(Qt::red);
	arrow2.setZ(5);
	arrow2.draw();
}




