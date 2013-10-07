#include <qmath.h>
#include <QCoreApplication>

#include "PhTools/PhDebug.h"
#include "GraphicTestView.h"
#include "PhTools/PhPictureTools.h"

GraphicTestView::GraphicTestView(QWidget *parent)
	: PhGraphicView( parent), _image(NULL), _font(NULL), _text(NULL), _rect(NULL), _loop(NULL), _yuvRect(NULL)
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
	_font = new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/Bedizen.ttf",50);
	if (! _font->init())
	{
		PHDEBUG << "_font not initialize";
		return false;
	}

	PHDEBUG << "Initialize _text";
	_text = new PhGraphicText(_font, "Test PhGraphicText");
	_text->setRect(50,135,250,125);
	_text->setColor(QColor(1, 1, 255));
	if (! _text->init())
	{
		PHDEBUG << "_text not initialize";
		return false;
	}
	_text->setX(280);

	PHDEBUG << "Initialize _rect";
	_rect = new PhGraphicSolidRect(100, 100, 75, 40);
	_rect->setColor(QColor(255, 1, 1));

	PHDEBUG << "Initialize _loop";
	_loop = new PhGraphicLoop();
	_loop->setX(100);
	_loop->setY(50);
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
	_image->setTextureCoordinate(1, 3);
	_image->draw();

	_text->setX(_text->getX() + 4);
	_text->draw();
	if(_text->getX() > this->width())
		_text->setX(0);
	if((_text->getX()+_text->getWidth()) < 0)
		_text->setX(this->width());

	_rect->draw();

	_loop->draw();

	_yuvRect->draw();
}




