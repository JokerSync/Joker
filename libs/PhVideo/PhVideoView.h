#ifndef PHVIDEOVIEW_H
#define PHVIDEOVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhVideoEngine.h"

class PhVideoView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit PhVideoView(QWidget *parent = 0);
	void setEngine(PhVideoEngine * videoEngine);

protected:
	bool init();
	void paint();

private:
	PhVideoEngine *_videoEngine;
};

#endif // PHVIDEOVIEW_H
