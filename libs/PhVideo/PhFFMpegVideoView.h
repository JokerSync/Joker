#ifndef PHFFMPEGVIDEOVIEW_H
#define PHFFMPEGVIDEOVIEW_H

#include <QWidget>
#include "PhVideoObject.h"
#include "PhGraphic/PhGraphicView.h"

#include "PhGraphic/PhGraphicTexturedRect.h"

struct AVFormatContext;
struct AVFrame;

class PhFFMpegVideoView : public PhGraphicView, public PhVideoObject
{
	Q_OBJECT
public:
	explicit PhFFMpegVideoView(QWidget *parent = 0);

	bool open(QString fileName);
signals:

protected:
	~PhFFMpegVideoView();
	bool init();
	void paint();
protected slots:
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);
	void checkVideoPosition();

private:
	AVFormatContext * _pFormatContext;
	AVFrame * _pFrame;
	PhGraphicTexturedRect _videoRect;
};

#endif // PHFFMPEGVIDEOVIEW_H
