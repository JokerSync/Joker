#ifndef PHVIDEOENGINE_H
#define PHVIDEOENGINE_H

extern "C" {
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

	void setFrameStamp(PhFrame frame);

	PhFrame frameStamp() { return _frameStamp;}

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

	QSettings *_settings;
	QString _fileName;
	PhClock _clock;
	PhFrame _frameStamp;

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
