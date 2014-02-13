/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

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

/*!
 * \brief The PhVideoEngine class
 *
 * It provide engine which compute the video from a file to an openGL texture
 */
class PhVideoEngine : public QObject
{
	Q_OBJECT
public:
	explicit PhVideoEngine(QObject *parent = 0);
	~PhVideoEngine();

	// Properties
	/*!
	 * \brief Get the video file name
	 * \return the video file name
	 */
	QString fileName() { return _fileName;}
	/*!
	 * \brief Get the clock
	 * \return the clock
	 */
	PhClock* clock() { return &_clock; }
	/*!
	 * \brief Get first frame
	 * \return the first frame of the video file
	 */
	PhFrame firstFrame() { return _firstFrame;}
	/*!
	 * \brief Get last frame
	 * \return the last frame of the video file
	 */
	PhFrame lastFrame() { return _firstFrame + length() - 1;}
	/*!
	 * \brief Get the length
	 * \return the length of the video
	 */
	PhFrame length();
	/*!
	 * \brief Get the codec name
	 * \return the codec name
	 */
	QString codecName();
	/*!
	 * \brief Get the width
	 * \return the PhVideoEngine width (not necessary the video width)
	 */
	int width();
	/*!
	 * \brief Get the height
	 * \return the PhVideoEngine height (not necessary the video height)
	 */
	int height();
	/*!
	 * \brief get frame per second
	 * \return the FPS of the video file
	 */
	float framePerSecond();
	/*!
	 * \brief Get refreshRate
	 * \return Return the refresh rate of the PhVideoEngine
	 */
	int refreshRate() { return _videoFrameTickCounter.frequency(); }

	/*!
	 * \brief Set the settings
	 * \param settings the desired QSettings
	 */
	void setSettings(QSettings *settings);
	/*!
	 * \brief Set first frame
	 * \param frame the new first frame
	 */
	void setFirstFrame(PhFrame frame);

	// Methods
	/**
	 * @brief Open a video file
	 * @param fileName A video file path
	 * @return True if the file was opened successfully, false otherwise
	 */
	bool open(QString fileName);
	/*!
	 * \brief Close
	 *
	 * Close the PhVideoEngine, freeing all objects
	 */
	void close();
	/*!
	 * \brief Prompt if the PhVideoEngine is ready
	 * \return True if the PhVideoEngine is ready, false otherwise
	 */
	bool ready();
	/*!
	 * \brief draw the video depending on the parameters
	 * \param x coordinates of the upperleft corner
	 * \param y coordinates of the upperleft corner
	 * \param w width
	 * \param h height
	 */
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
