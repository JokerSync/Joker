#ifndef SECONDSCREENWINDOW_H
#define SECONDSCREENWINDOW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhVideo/PhVideoEngine.h"

class SecondScreenWindow : public PhGraphicView
{
	Q_OBJECT
public:
	explicit SecondScreenWindow(PhVideoEngine *videoEngine, PhGraphicView *shareWidget);

signals:

private slots:
	void onPaint(int width, int height);

private:
	PhVideoEngine *_videoEngine;
};

#endif // SECONDSCREENWINDOW_H
