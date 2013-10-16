#ifndef PHFFMPEGVIDEOVIEW_H
#define PHFFMPEGVIDEOVIEW_H

#include <QTimer>

#include "PhVideoObject.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}


class PhFFMpegVideoView : public PhVideoObject
{
public:
	PhFFMpegVideoView();

	bool open(QString fileName);
signals:

protected:
	~PhFFMpegVideoView();

	bool goToFrame(PhFrame frame);
	virtual bool drawFrame(AVFrame * avFrame) = 0;
	AVCodecContext * _pCodecContext;
	struct SwsContext * _pSwsCtx;
private:

	AVFormatContext * _pFormatContext;
	int _videoStream;
	AVFrame * _pFrame;
};

#endif // PHFFMPEGVIDEOVIEW_H
