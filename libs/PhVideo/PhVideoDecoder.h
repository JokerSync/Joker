/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEODECODER_H
#define PHVIDEODECODER_H

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

#include "PhVideoBuffer.h"

/**
 * @brief The video decoder
 *
 * It decodes frames from a video file.
 * The frames are requested by the engine and provided to the engine asynchronously,
 * using signal and slots. This allows the decoder to work in a separate thread without blocking the GUI.
 */
class PhVideoDecoder : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief PhVideoEngine constructor
	 */
	explicit PhVideoDecoder();

	~PhVideoDecoder();

public slots:
	/**
	 * @brief Open a video file
	 * @param fileName A video file path
	 */
	void open(QString fileName);

	/**
	 * @brief Close
	 *
	 * Close the PhVideoDecoder, freeing all objects
	 */
	void close();

	/**
	 * @brief decode a video frame
	 * @param buffer the requested frame
	 */
	void decodeFrame(PhVideoBuffer *buffer);

	/**
	 * @brief cancel a frame request
	 * @param frame the frame describing the request
	 */
	void cancelFrameRequest(PhVideoBuffer *frame);

	/**
	 * @brief Signal sent when the deinterlace settings change
	 * @param deinterlace Whether the video should be deinterlaced
	 */
	void setDeinterlace(bool deinterlace);

signals:
	/**
	 * @brief Signal sent when a frame has been decoded
	 * @param buffer The frame where the decoded frame is
	 */
	void frameAvailable(PhVideoBuffer *buffer);

	/**
	 * @brief Signal sent when a frame request has been cancelled
	 * @param frame The frame describing the request
	 */
	void frameCancelled(PhVideoBuffer *frame);

	/**
	 * @brief Signal sent when the decoder is ready
	 * @param length The length of the video file
	 * @param framePerSecond The frame per second
	 * @param timeIn The time in of the video file
	 * @param width The width of the frame
	 * @param height The height of the frame
	 * @param codecName The codec name
	 */
	void opened(PhTime length, double framePerSecond, PhTime timeIn, int width, int height, QString codecName);

	/**
	 * @brief Signal sent when the decoder failed to open the file
	 */
	void openFailed();

private:
	bool ready();
	double framePerSecond();
	PhTime length();
	void frameToRgb(AVFrame *avFrame, PhVideoBuffer *buffer);
	int width();
	int height();
	QString codecName();
	PhTime timeIn();

	int64_t PhTime_to_AVTimestamp(PhTime time);
	PhTime AVTimestamp_to_PhTime(int64_t timestamp);

	QString _fileName;
	PhTimeCodeType _tcType;

	AVFormatContext * _formatContext;
	AVStream *_videoStream;
	AVFrame * _videoFrame;
	struct SwsContext * _swsContext;
	PhTime _currentTime;

	bool _useAudio;
	AVStream *_audioStream;
	AVFrame * _audioFrame;

	bool _deinterlace;
	bool _recursive;

	QList<PhVideoBuffer *> _requestedFrames;
};

#endif // PHVIDEODECODER_H
