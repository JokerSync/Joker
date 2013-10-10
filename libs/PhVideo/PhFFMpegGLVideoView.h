#ifndef PHFFMPEGGLVIDEOVIEW_H
#define PHFFMPEGGLVIDEOVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicTexturedRect.h"
#include "PhFFMpegVideoView.h"

class PhFFMpegGLVideoView : public PhGraphicView, public PhFFMpegVideoView
{
	Q_OBJECT
public:
	explicit PhFFMpegGLVideoView(QWidget *parent = 0);

protected slots:
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);
	void checkVideoPosition();

protected:
	bool drawFrame(AVFrame *avFrame);
	bool init();
	void paint();

private:
	struct SwsContext * _pSwsCtx;
	PhGraphicTexturedRect videoRect;
	uint8_t * _rgb;
};

#endif // PHFFMPEGGLVIDEOVIEW_H
