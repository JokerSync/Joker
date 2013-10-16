#ifndef PHVIDEOENGINE_H
#define PHVIDEOENGINE_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <QObject>
#include "PhTools/PhClock.h"
#include "PhGraphic/PhGraphicTexturedRect.h"

class PhVideoEngine : public QObject
{
	Q_OBJECT
public:
	explicit PhVideoEngine(QObject *parent = 0);
	~PhVideoEngine();

	/**
	 * @brief Open a video file
	 * @param fileName A video file path
	 * @return True if the file was opened successfully, false otherwise
	 */
	bool open(QString fileName);
	void drawVideo(int x, int y, int w, int h);

	PhClock* clock() { return &_clock; };

	void setFrameStamp(PhFrame frame);

	PhFrame frameStamp() { return _frameStamp;};

private:
	bool goToFrame(PhFrame frame);

	PhClock _clock;
	PhFrame _frameStamp;

	AVCodecContext * _pCodecContext;
	struct SwsContext * _pSwsCtx;
	AVFormatContext * _pFormatContext;
	int _videoStream;
	AVFrame * _pFrame;
	PhGraphicTexturedRect videoRect;
	uint8_t * _rgb;
};

#endif // PHVIDEOENGINE_H
