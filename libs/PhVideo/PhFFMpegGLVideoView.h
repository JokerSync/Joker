#ifndef PHFFMPEGGLVIDEOVIEW_H
#define PHFFMPEGGLVIDEOVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhFFMpegVideoView.h"

class PhFFMpegGLVideoView : public PhGraphicView, public PhFFMpegVideoView
{
	Q_OBJECT
public:
	explicit PhFFMpegGLVideoView(QWidget *parent = 0);

signals:

public slots:

};

#endif // PHFFMPEGGLVIDEOVIEW_H
