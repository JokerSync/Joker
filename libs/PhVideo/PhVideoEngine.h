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
#include <portaudio.h>

#include "PhTools/PhClock.h"
#include "PhTools/PhTickCounter.h"
#include "PhGraphic/PhGraphicTexturedRect.h"


class PhVideoEngine : public QObject
{
	Q_OBJECT
public:
	explicit PhVideoEngine(bool useAudio = true, QObject *parent = 0);
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

	bool init(int nbChannels, AVSampleFormat sampleFormat, int sampleRate, int framePerBuffer, QString deviceName = "");


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
	AVFrame * _videoFrame;
	struct SwsContext * _pSwsCtx;
	PhGraphicTexturedRect videoRect;
	uint8_t * _rgb;
	PhFrame _currentFrame;

	QElapsedTimer _testTimer;
	PhTickCounter _videoFrameTickCounter;

	bool _useAudio;
	AVStream *_audioStream;
	AVFrame * _audioFrame;

	PaStreamCallbackResult processAudio(void *outputBuffer,
							unsigned long framesPerBuffer);
	static int audioCallback( const void *inputBuffer, void *outputBuffer,
							   unsigned long framesPerBuffer,
							   const PaStreamCallbackTimeInfo* timeInfo,
							   PaStreamCallbackFlags statusFlags,
							   void *userData );
	PaStream *stream;
	float data;

};

#endif // PHVIDEOENGINE_H
