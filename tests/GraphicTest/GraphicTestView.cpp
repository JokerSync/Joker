#include "GraphicTestView.h"
#include "math.h"

GraphicTestView::GraphicTestView(QWidget *parent)
	: PhGraphicView( parent), _image(NULL), _font(NULL), _text(NULL), _rect(NULL)
{

}

bool GraphicTestView::init()
{
	playEnable = 0;
	textSpeed = 0;
	enableDisplayImage = false;
	enableDisplayRect = false;
	enableDisplayText = false;

	qDebug() << "GraphicTestView::init";
	if(_image == NULL)
	{
		qDebug() << "Initialize _image";
		_image = new PhGraphicImage();
		_image->setFilename("look.png");
		_image->setTextureCoordinate(1,1);
		_image->setRect(50,0,250,125);
		if (! _image->init())
			qDebug() << "_image not initialize";
	}



	if(_font == NULL)
	{
		qDebug() << "Initialize _font";
		_font = new PhFont("Bedizen.ttf",50);
		if (! _font->init())
			qDebug() << "_font not initialize";
	}

	if(_text == NULL)
	{
		qDebug() << "Initialize _text";
		_text = new PhGraphicText(_font, "Test PhGraphicText");
		_text->setRect(50,135,250,125);
		_text->setColor(new QColor(100, 254, 0));
		if (! _text->init())
			qDebug() << "_text not initialize";

		_text->setX(280);
	}

	if(_rect == NULL)
	{
		qDebug() << "Initialize _text";
		_rect = new PhGraphicSolidRect(100, 100, 75, 40, 1, new QColor(200, 128, 0));
	}

	return true;
}

void GraphicTestView::paint()
{
	if  (enableDisplayImage == true)
	{
		if(_image != NULL)
		{
			_image->setTextureCoordinate(1, 3);
			_image->draw();
		}
	}

	if  (enableDisplayText == true)
	{
		//qDebug() << "text content : " << _text->getContent();

		if (textSpeed == 0)
		{
			if(_text != NULL)
			{
				_text->draw();
			}
		}
		else
		{
			if(_text != NULL)
			{
				_text->setX(_text->getX()+(2*textSpeed));
				_text->draw();
				if(_text->getX() > this->width())
					_text->setX(0);
				if((_text->getX()+_text->getWidth()) < 0)
					_text->setX(this->width());

			}
		}
	}

	if  (enableDisplayRect == true)
	{
		_rect->draw();
	}
}


void GraphicTestView::play()
{
	if (playEnable == 1)
	{
		playEnable = 0;
		textSpeed = 0;
	}
	else
	{
		playEnable = 1;
		textSpeed = 1;
	}
}

void GraphicTestView::fastBackward()
{
	textSpeed = fabs(textSpeed) * (-2);
	if(fabs(textSpeed) > 8)
		textSpeed = -8;
}

void GraphicTestView::fastForward()
{
	textSpeed = fabs(textSpeed) * 2;
	if(fabs(textSpeed) > 8)
		textSpeed = 8;
}

void GraphicTestView::displayText()
{
	if (enableDisplayText == false)
		enableDisplayText = true;
	else
		enableDisplayText = false;
}


void GraphicTestView::displayImage()
{
	if (enableDisplayImage == false)
		enableDisplayImage = true;
	else
		enableDisplayImage = false;
}

void GraphicTestView::displayRect()
{
	if (enableDisplayRect == false)
		enableDisplayRect = true;
	else
		enableDisplayRect = false;
}



