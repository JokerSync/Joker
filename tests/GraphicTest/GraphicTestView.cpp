#include "GraphicTestView.h"

GraphicTestView::GraphicTestView(QWidget *parent, QString name) : PhGraphicView( parent , name)
{
}

bool GraphicTestView::init()
{
	initializeGL();
	_image.setFilename("look.png");
	_image.init();
}

void GraphicTestView::paint()
{
	paintGL();

}
