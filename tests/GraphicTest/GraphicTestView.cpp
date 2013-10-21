#include <qmath.h>
#include <QCoreApplication>

#include "PhTools/PhDebug.h"
#include "GraphicTestView.h"
#include "PhTools/PhPictureTools.h"

GraphicTestView::GraphicTestView(QWidget *parent)
	: PhGraphicView( parent),
	  _image(NULL),
	  _font1(NULL),
	  _font2(NULL),
	  _text1(NULL),
	  _text2(NULL),
	  _rect(NULL),
	  _loop(NULL),
	  _yuvRect(NULL)
{

}

bool GraphicTestView::init()
{
	PHDEBUG << "Initialize _image";
	_image = new PhGraphicImage();
	_image->setFilename(QCoreApplication::applicationDirPath() + "/../Resources/look.png");
	_image->setTextureCoordinate(1,1);
	_image->setRect(50,0,250,125);
	if (! _image->init())
	{
		PHDEBUG << "_image not initialize";
		return false;
	}

	PHDEBUG << "Initialize _font";
	_font1 = new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/SWENSON.TTF");
	if (! _font1->init())
	{
		PHDEBUG << "_font not initialize";
		return false;
	}
	_font2 = new PhFont("/Library/Fonts/Arial.ttf");
	if (! _font2->init())
	{
		PHDEBUG << "_font not initialize";
		return false;
	}

	PHDEBUG << "Initialize _text";
	_text1 = new PhGraphicText(_font1, "The party is over!");
	_text1->setRect(50, 100, 500, 100);
	_text1->setColor(QColor(255, 0, 0));
	_text1->setZ(-1);

	_text2 = new PhGraphicText(_font2, "The party is over!");
	_text2->setRect(50, 300, 500, 100);
	_text2->setColor(QColor(255, 0, 0));
	_text2->setZ(-1);

	PHDEBUG << "Initialize _rect";
	_rect = new PhGraphicSolidRect(100, 100, 75, 40);
	_rect->setColor(QColor(0, 255, 0));
	_rect->setZ(-2);

	PHDEBUG << "Initialize _loop";
	_loop = new PhGraphicLoop();
	_loop->setX(100);
	_loop->setY(50);
	_loop->setZ(-3);
	_loop->setWidth(120);
	_loop->setHeight(100);
	_loop->setHThick(5);
	_loop->setCrossHeight(60);
	_loop->setColor(QColor(1, 255, 1));
	if (! _loop->init())
	{
		PHDEBUG << "_loop not initialize";
		return false;
	}

	int w = 50;
	int h = 50;
	unsigned char * yuv = PhPictureTools::generateYUVPattern(w, h);
	_yuvRect = new PhGraphicTexturedRect(20, 300, 150, 100);
	_yuvRect->createTextureFromYUVBuffer(yuv, w, h);

	return true;
}

void GraphicTestView::paint()
{
	glClearColor(1.0f,1.0f,1.0f, 1.0f);

//	_image->setTextureCoordinate(1, 3);
	_image->draw();

	_rect->setColor(QColor(0, 255, 0));
	_rect->setRect(50, 100, 500, 75);
	_rect->draw();

	_rect->setColor(QColor(0, 0, 255));
	_rect->setRect(50, 175, 500, 25);
	_rect->draw();

	_text1->setRect(50, 100, 500, 100);
	_text1->draw();

	_text2->draw();




//	_text->setX(_text->getX() + 4);
//	if(_text->getX() > this->width())
//		_text->setX(0);
//	if((_text->getX()+_text->getWidth()) < 0)
//		_text->setX(this->width());


//	_loop->draw();

	//_yuvRect->draw();
}




