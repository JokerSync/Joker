/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOENGINE_H
#define PHVIDEOENGINE_H

#include "PhSync/PhClock.h"
#include "PhTools/PhTickCounter.h"
#include "PhGraphic/PhGraphicTexturedRect.h"

#include "PhVideoSettings.h"
#include "PhVideoFrame.h"

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
	 * @brief whether the time corresponds to the frame that we currently have
	 * @param time the current time
	 * @return True if the frame is current
	 */
	bool isFrameCurrent(PhTime time);

	/**
	 * @brief whether the time corresponds to the frame that we have requested
	 * @param time the current time
	 * @return True if the frame has already been requested
	 */
	bool isFrameRequested(PhTime time);

public slots:
	/**
	 * @brief Handle a frame that has just been decoded
	 * @param frame the decoded frame
	 */
	void frameAvailable(PhVideoFrame *frame);

	/**
	 * @brief Handle the signal that a frame request has been cancelled in the decoder
	 * @param frame the frame describing the request
	 */
	void frameCancelled(PhVideoFrame *frame);

	/**
	 * @brief Handle the signal that the video file has been opened in the decoder
	 * @param length the length of the video file
	 * @param framePerSecond the frame per second
	 * @param timeIn the time in of the video file
	 * @param width the width of the frame
	 * @param height the height of the frame
	 * @param codecName the codec name
	 */
	void decoderOpened(PhTime length, double framePerSecond, PhTime timeIn, int width, int height, QString codecName);

	/**
	 * @brief Handle the signal sent when the decoder failed to open the file
	 */
	void openInDecoderFailed();

signals:
	/**
	 * @brief Signal sent upon a different timecode type message
	 * @param tcType A timecode type value.
	 */
	void timeCodeTypeChanged(PhTimeCodeType tcType);

	/**
	 * @brief Signal sent to ask the decoder to decode a video frame
	 * @param frame the requested frame
	 */
	void decodeFrame(PhVideoFrame *frame);

	/**
	 * @brief Signal sent to the decoder to open a video file
	 * @param fileName A video file path
	 */
	void openInDecoder(QString fileName);

	/**
	 * @brief Signal sent to the decoder to close the file
	 */
	void closeInDecoder();

	/**
	 * @brief Signal sent when the engine is ready
	 * @param success Whether the video was opened successfully
	 */
	void opened(bool success);

	/**
	 * @brief Signal sent to cancel a frame request
	 * @param frame the frame describing the request
	 */
	void cancelFrameRequest(PhVideoFrame *frame);

	/**
	 * @brief Signal sent when the deinterlace settings change
	 * @param deinterlace Whether the video should be deinterlaced
	 */
	void deinterlaceChanged(bool deinterlace);

	/**
	 * @brief Signal sent when a new frame is displayed on screen.
	 * This is used by the tests, where some form of synchronization is required.
	 * @param frameTime the time of the frame being displayed
	 */
	void newFrameDisplayed(PhTime frameTime);

private:
	void requestFrame(PhTime time);
	void requestFrames(PhTime time);
	void cleanupFramePools();
	void showFrame(PhVideoFrame *frame);
	PhVideoFrame *frameFromPool(PhTime time);
	PhTime clockTime();

	PhVideoSettings *_settings;
	QString _fileName;
	PhTimeCodeType _tcType;
	PhClock _clock;
	PhTime _length;
	PhTime _timeIn;
	double _framePerSecond;
	int _width;
	int _height;
	QString _codecName;
	bool _ready;

	PhGraphicTexturedRect _videoRect;
	PhTime _currentFrameTime;

	PhTickCounter _videoFrameTickCounter;

	bool _deinterlace;

	QList<PhVideoFrame*> _recycledframePool;
	QList<PhVideoFrame*> _requestedframePool;
	QList<PhVideoFrame*> _cancelledframePool;
	QList<PhVideoFrame*> _decodedFramePool;

	QThread _decoderThread;
};

#endif // PHVIDEOENGINE_H
