#include "GraphicTestView.h"

GraphicTestView::GraphicTestView(QWidget *parent, QString name)
	: PhGraphicView( parent , name ) , _image(NULL)
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
		_image->setRect(0,0,200,150);
		if (! _image->init())
			qDebug() << "_image not initialize";
	}
}

void GraphicTestView::paint()
{
	qDebug() << "GraphicTestView::paint";
	if(_image != NULL)
		_image->draw();
}


