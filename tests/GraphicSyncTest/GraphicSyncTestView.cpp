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
#if defined(Q_OS_MAC)
	font.setFontFile("/Library/Fonts/Arial.ttf");
#else
	font.setFontFile("C:/Windows/Fonts/Arial.ttf");
#endif
	text.setRect(0, 0, 300,  50);
	return true;
}

void GraphicSyncTestView::paint()
{
	text.draw();
}
