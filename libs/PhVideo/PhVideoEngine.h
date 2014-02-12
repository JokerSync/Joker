#ifndef PHVIDEOENGINE_H
#define PHVIDEOENGINE_H

extern "C" {
// see http://code.google.com/p/ffmpegsource/issues/detail?id=11#c13
// and http://code.google.com/p/ffmpegsource/issues/detail?id=11#c23

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <QObject>
#include <QElapsedTimer>
#include <QSettings>

#include "PhTools/PhClock.h"
#include "PhTools/PhTickCounter.h"
#include "PhGraphic/PhGraphicTexturedRect.h"

class PhVideoEngine : public QObject
{
	Q_OBJECT
public:
	explicit PhVideoEngine(QObject *parent = 0);
	~PhVideoEngine();

	// Properties
	QString fileName() { return _fileName;}

	PhClock* clock() { return &_clock; }

	int width();

	int height();

	float framePerSecond();

	QString codecName();

	void setFirstFrame(PhFrame frame);

	PhFrame firstFrame() { return _firstFrame;}

	PhFrame lastFrame() { return _firstFrame + length() - 1;}

	PhFrame length();

	int refreshRate() { return _videoFrameTickCounter.frequency(); }

	bool ready();

	// Methods
	/**
	 * @brief Open a video file
	 * @param fileName A video file path
	 * @return True if the file was opened successfully, false otherwise
	 */
	bool open(QString fileName);
	void close();

	void setSettings(QSettings *settings);

	void drawVideo(int x, int y, int w, int h);

private:
	bool goToFrame(PhFrame frame);
	int64_t frame2time(PhFrame f);
	PhFrame time2frame(int64_t t);

	QSettings *_settings;
	QString _fileName;
	PhClock _clock;
	PhFrame _firstFrame;

	AVFormatContext * _pFormatContext;
	AVStream *_videoStream;
	AVCodecContext * _pCodecContext;
	AVFrame * _pFrame;
	struct SwsContext * _pSwsCtx;
	PhGraphicTexturedRect videoRect;
	uint8_t * _rgb;
	PhFrame _currentFrame;

	QElapsedTimer _testTimer;
	PhTickCounter _videoFrameTickCounter;


};

#endif // PHVIDEOENGINE_H
