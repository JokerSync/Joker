/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOENGINE_H
#define PHVIDEOENGINE_H

extern "C" {
#ifndef INT64_C
/** see http://code.google.com/p/ffmpegsource/issues/detail?id=11#c13 */
#define INT64_C(c) (c ## LL)
/** and http://code.google.com/p/ffmpegsource/issues/detail?id=11#c23 */
#define UINT64_C(c) (c ## ULL)
#endif

#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <QObject>
#include <QElapsedTimer>
#include <QSemaphore>
#include <QThread>

#include "PhTools/PhClock.h"
#include "PhTools/PhTickCounter.h"
#include "PhGraphic/PhGraphicTexturedRect.h"

#include "PhVideoSettings.h"

#include "PhAVDecoder.h"

/**
 * @brief The video engine
 *
 * It provide engine which compute the video from a file to an openGL texture.
 */
class PhVideoEngine : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief PhVideoEngine constructor
	 * @param useAudio Shall decode audio frame?
	 * @param parent The parent object
	 */
	explicit PhVideoEngine(bool useAudio, QObject *parent = 0);
	~PhVideoEngine();

	// Properties
	/**
	 * @brief Get the video file name
	 * @return the video file name
	 */
	QString fileName() {
		return _fileName;
	}
	/**
	 * @brief Get the clock
	 * @return the clock
	 */
	PhClock* clock() {
		return &_clock;
	}
	/**
	 * @brief Get first frame
	 * @return the first frame of the video file
	 */
	PhFrame firstFrame() {
		return _firstFrame;
	}
	/**
	 * @brief Get last frame
	 * @return the last frame of the video file
	 */
	PhFrame lastFrame() {
		return _firstFrame + length() - 1;
	}
	/**
	 * @brief Get the length
	 * @return the length of the video
	 */
	PhFrame length();
	/**
	 * @brief Get the codec name
	 * @return the codec name
	 */
	QString codecName();
	/**
	 * @brief Get the width
	 * @return the PhVideoEngine width (not necessary the video width)
	 */
	int width();
	/**
	 * @brief Get the height
	 * @return the PhVideoEngine height (not necessary the video height)
	 */
	int height();
	/**
	 * @brief get frame per second
	 * @return the FPS of the video file
	 */
	float framePerSecond();

	/**
	 * @brief Set the settings
	 * @param settings the desired settings
	 */
	void setSettings(PhVideoSettings *settings);
	/**
	 * @brief Set first frame
	 * @param frame the new first frame
	 */
	void setFirstFrame(PhFrame frame);

	// Methods
	/**
	 * @brief Open a video file
	 * @param fileName A video file path
	 * @return True if the file was opened successfully, false otherwise
	 */
	bool open(QString fileName);
	/**
	 * @brief Close
	 *
	 * Close the PhVideoEngine, freeing all objects
	 */
	void close();
	/**
	 * @brief Prompt if the PhVideoEngine is ready
	 * @return True if the PhVideoEngine is ready, false otherwise
	 */
	bool ready();
	/**
	 * @brief draw the video depending on the parameters
	 * @param x coordinates of the upperleft corner
	 * @param y coordinates of the upperleft corner
	 * @param w width
	 * @param h height
	 */
	void drawVideo(int x, int y, int w, int h);

public slots:
	void errorString(QString);

private:
	int64_t frame2time(PhFrame f);
	PhFrame time2frame(int64_t t);

	PhFrame _firstFrame;
	PhFrame _oldFrame;
	PhVideoSettings *_settings;
	QString _fileName;
	PhClock _clock;

	QMap<PhFrame, uint8_t * > * _nextImages;

	PhAVDecoder * _decoder;
	QSemaphore * _framesProcessed;
	QSemaphore * _framesFree;
	QThread * _thread;

	AVStream *_videoStream;
	AVFormatContext * _pFormatContext;

	PhGraphicTexturedRect * _videoRect;






};

#endif // PHVIDEOENGINE_H
