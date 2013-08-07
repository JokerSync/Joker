#include "GraphicTestView.h"

GraphicTestView::GraphicTestView(QWidget *parent, QString name) : PhGraphicView( parent , name)
{
}

bool GraphicTestView::init()
{
	qDebug() << "GraphicTestView::init";
	_image.setFilename("look.png");
	_image.init();
}

void GraphicTestView::paint()
{
	qDebug() << "GraphicTestView::paint";
	_image.draw();
}


