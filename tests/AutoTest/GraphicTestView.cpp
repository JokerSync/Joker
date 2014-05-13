/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QWindow>

#include "GraphicTestView.h"

GraphicTestView::GraphicTestView(QWidget *parent) :
	PhGraphicView(parent)
{
	int ratio = this->windowHandle()->devicePixelRatio();
	setGeometry(0, 0, 32 / ratio, 32 / ratio);
}

QImage GraphicTestView::saveBuffer()
{
	return this->grabFrameBuffer();
}

bool GraphicTestView::init()
{
	emit initSignal();
}

void GraphicTestView::paint()
{
	//int ratio = this->windowHandle()->devicePixelRatio();
	int ratio = 1;
	int width = this->width() * ratio;
	int height = this->width() * ratio;

	emit draw(width, height);
}
