#ifndef PHFFMPEGVIDEOVIEW_H
#define PHFFMPEGVIDEOVIEW_H

#include <QWidget>
#include <QTimer>

#include "PhVideoObject.h"

#include "PhGraphic/PhGraphicTexturedRect.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

}


class PhFFMpegVideoView : public QWidget, public PhVideoObject
{
	Q_OBJECT
public:
	explicit PhFFMpegVideoView(QWidget *parent = 0);

	bool open(QString fileName);
signals:

protected:
	~PhFFMpegVideoView();

	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *event);

protected slots:
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);
	void checkVideoPosition();

private:
	bool goToFrame(PhFrame frame);

	AVFormatContext * _pFormatContext;
	int _videoStream;
	AVCodecContext * _pCodecContext;
	AVFrame * _pFrame;
	struct SwsContext * _pSwsCtx;
	PhGraphicTexturedRect videoRect;

	QImage * _image;
	uint8_t *_rgb;
};

#endif // PHFFMPEGVIDEOVIEW_H
