#include "GraphicTestView.h"

GraphicTestView::GraphicTestView(QWidget *parent, QString name)
	: PhGraphicView( parent , name ) , _image(NULL), _text(NULL)
{

}

bool GraphicTestView::init()
{
	qDebug() << "GraphicTestView::init";
	if(_image == NULL)
	{
		qDebug() << "Initialize _image";
		_image = new PhGraphicImage;
		_image->setFilename("look.png");
		_image->setRect(50,0,250,125);
		if (! _image->init())
			qDebug() << "_image not initialize";
	}

	if(_text == NULL)
	{
		qDebug() << "Initialize _text";
		_text = new PhGraphicText;
		_text->setContent("Test PhGraphicText");
		_text->setRect(50,135,250,125);
		if (! _text->init())
			qDebug() << "_text not initialize";

	}
}

void GraphicTestView::paint()
{
	qDebug() << "GraphicTestView::paint";
	if(_image != NULL)
		_image->draw();

	if(_text != NULL)
		_text->draw();
}


