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
#include <QVideoFrame>

#include "PhSync/PhClock.h"
#include "PhTools/PhTickCounter.h"
#include "PhGraphic/PhGraphicTexturedRect.h"

#include "PhVideoSettings.h"

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
	 * @param settings The settings
	 */
	PhVideoEngine(PhVideoSettings *settings);

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
	 * @brief The video timecode type
	 * @return A timecode type value
	 */
	PhTimeCodeType timeCodeType() {
		return _tcType;
	}

	/**
	 * @brief Get the clock
	 * @return the clock
	 */
	PhClock* clock() {
		return &_clock;
	}

	/**
	 * @brief Get the first frame of the video file
	 * @return A frame value
	 */
	PhFrame frameIn() {
		return _frameIn;
	}

	/**
	 * @brief Set first frame
	 * @param frameIn the new first frame
	 */
	void setFrameIn(PhFrame frameIn);

	/**
	 * @brief Get the starting time of the video file
	 * @return A time value
	 */
	PhTime timeIn() {
		return _frameIn * PhTimeCode::timePerFrame(_tcType);
	}

	/**
	 * @brief Set the video starting time
	 * @param timeIn A time value.
	 */
	void setTimeIn(PhTime timeIn);

	/**
	 * @brief Get last frame
	 * @return the last frame of the video file
	 */
	PhFrame frameOut() {
		return _frameIn + frameLength() - 1;
	}

	/**
	 * @brief Get the video ending time
	 * @return A time value.
	 */
	PhTime timeOut() {
		return frameOut() * PhTimeCode::timePerFrame(_tcType);
	}

	/**
	 * @brief Get the video length in frame
	 * @return A frame value
	 */
	PhFrame frameLength();

	/**
	 * @brief Get the video length
	 * @return A time value
	 */
	PhTime length();

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
	 * @brief Get refreshRate
	 * @return Return the refresh rate of the PhVideoEngine
	 */
	int refreshRate() {
		return _videoFrameTickCounter.frequency();
	}

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
	 * @brief Check if video shall be deinterlace
	 * @return True if deinterlace false otherwise
	 */
	bool deinterlace() {
		return _deinterlace;
	}

	/**
	 * @brief Set the video deinterlace mode
	 * @param deinterlace True if deinterlace false otherwise
	 */
	void setDeinterlace(bool deinterlace);

	/**
	 * @brief Retrieve the video filtering
	 * @return True if bilinear filtering is enabled
	 */
	bool getBilinearFiltering() {
		return _bilinearFiltering;
	}

	/**
	 * @brief Enable or disable the video bilinear filtering
	 * Video bilinear filtering is enabled by default.
	 * @param bilinear True to enable bilinear filtering
	 */
	void setBilinearFiltering(bool bilinear);

	/**
	 * @brief draw the video depending on the parameters
	 * @param x coordinates of the upperleft corner
	 * @param y coordinates of the upperleft corner
	 * @param w width
	 * @param h height
	 */
	void drawVideo(int x, int y, int w, int h);

	/**
	 * @brief decode the video for the current time (may not do anything if not needed)
	 */
	void decodeVideo();

signals:
	/**
	 * @brief Signal sent upon a different timecode type message
	 * @param tcType A timecode type value.
	 */
	void timeCodeTypeChanged(PhTimeCodeType tcType);

	void newVideoContentProduced(const QVideoFrame &frame);

private:
	bool goToFrame(PhFrame frame);
	int64_t frame2time(PhFrame f);
	PhFrame time2frame(int64_t t);

	PhVideoSettings *_settings;
	QString _fileName;
	PhTimeCodeType _tcType;
	PhClock _clock;
	PhFrame _frameIn;

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

	bool _deinterlace;
	bool _bilinearFiltering;
};

#endif // PHVIDEOENGINE_H
