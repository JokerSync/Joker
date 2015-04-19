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
}

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
	explicit PhVideoEngine(PhVideoSettings *settings);

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
	 * @brief Get the starting time of the video file
	 * @return A time value
	 */
	PhTime timeIn() {
		return _timeIn;
	}

	/**
	 * @brief Set the video starting time
	 * @param timeIn A time value.
	 */
	void setTimeIn(PhTime timeIn);

	/**
	 * @brief Get the video ending time
	 * @return A time value.
	 */
	PhTime timeOut() {
		return _timeIn + length();
	}

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
	 * @brief Get average number of frame per second
	 * @return A double value.
	 */
	double framePerSecond();
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
	bool bilinearFiltering();

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
	 * @brief whether the time corresponds to the frame that we currently have (or that we have requested)
	 * @param time the current time
	 * @return True if the frame is available
	 */
	bool isFrameAvailable(PhTime time);

public slots:
	/**
	 * @brief Handle a frame that has just been decoded
	 * @param time the time of the decoded frame (with origin at the start of video file)
	 * @param rgb the buffer where the decoded frame is
	 * @param width the width of the frame
	 * @param height the height of the frame
	 */
	void frameAvailable(PhTime time, uint8_t *rgb, int width, int height);

signals:
	/**
	 * @brief Signal sent upon a different timecode type message
	 * @param tcType A timecode type value.
	 */
	void timeCodeTypeChanged(PhTimeCodeType tcType);

	/**
	 * @brief Signal sent to ask the decoder to decode a video frame
	 * @param time the time of the requested frame (with origin at the start of video file)
	 * @param rgb the buffer where to output the decoded frame
	 * @param deinterlace whether the frame is to be deinterlaced
	 */
	void decodeFrame(PhTime time, uint8_t *rgb, bool deinterlace);

	/**
	 * @brief Signal sent to the decoder to open a video file
	 * @param fileName A video file path
	 */
	void openInDecoder(QString fileName);

	/**
	 * @brief Signal sent to the decoder to close the file
	 */
	void closeInDecoder();

private:
	void requestFrame(PhTime time);
	int64_t PhTime_to_AVTimestamp(PhTime time);
	PhTime AVTimestamp_to_PhTime(int64_t timestamp);

	PhVideoSettings *_settings;
	QString _fileName;
	PhTimeCodeType _tcType;
	PhClock _clock;
	PhTime _timeIn;

	AVFormatContext * _formatContext;
	AVStream *_videoStream;
	PhGraphicTexturedRect _videoRect;
	PhTime _currentTime;

	PhTickCounter _videoFrameTickCounter;

	bool _useAudio;
	AVStream *_audioStream;
	AVFrame * _audioFrame;

	bool _deinterlace;

	QList<uint8_t *> _rgbBufferList;
	QList<bool> _bufferUsageList;
	QList<int> _bufferSizeList;

	QThread _decoderThread;
};

#endif // PHVIDEOENGINE_H
