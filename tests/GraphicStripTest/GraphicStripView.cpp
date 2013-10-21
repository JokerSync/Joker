#include "GraphicStripView.h"

GraphicStripView::GraphicStripView(QWidget *parent) :
	PhGraphicView(parent), _strip(parent)
{
}

bool GraphicStripView::init()
{
	return _strip.init();
}

void GraphicStripView::paint()
{
	_strip.draw(0, 0, this->width(), this->height());
}
