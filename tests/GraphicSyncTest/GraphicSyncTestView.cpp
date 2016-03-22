#include "GraphicSyncTestView.h"
#include <QtGlobal>

GraphicSyncTestView::GraphicSyncTestView(QWidget *parent) :
	PhGraphicView(parent),
	text(&font, "waiting")
{
}

void GraphicSyncTestView::setText(QString content)
{
	text.setContent(content);
}

bool GraphicSyncTestView::init()
{
	font.setFamily("Arial");
	text.setRect(0, 0, 300,  50);
	return true;
}

void GraphicSyncTestView::paint()
{
	text.draw();
}
