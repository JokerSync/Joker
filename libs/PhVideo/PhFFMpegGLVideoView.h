#ifndef PHFFMPEGGLVIDEOVIEW_H
#define PHFFMPEGGLVIDEOVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhVideoEngine.h"

class PhFFMpegGLVideoView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit PhFFMpegGLVideoView(QWidget *parent = 0);
	void setEngine(PhVideoEngine * videoEngine);

protected:
	bool init();
	void paint();

private:
	PhVideoEngine *_videoEngine;
};

#endif // PHFFMPEGGLVIDEOVIEW_H
