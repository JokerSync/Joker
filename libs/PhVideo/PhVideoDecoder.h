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

#include "PhSync/PhClock.h"

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
	 * @param time the time of the requested frame (with origin at the start of video file)
	 * @param rgb the buffer where to output the decoded frame
	 * @param deinterlace whether the frame is to be deinterlaced
	 */
	void decodeFrame(PhTime time, uint8_t *rgb, bool deinterlace);

signals:
	/**
	 * @brief Signal sent when a frame has been decoded
	 * @param time the time of the decoded frame (with origin at the start of video file)
	 * @param rgb the buffer where the decoded frame is
	 * @param width the width of the frame
	 * @param height the height of the frame
	 */
	void frameAvailable(PhTime time, uint8_t *rgb, int width, int height);

private:
	bool ready();
	double framePerSecond();
	PhTime length();
	void frameToRgb(uint8_t *rgb, bool deinterlace);

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
};

#endif // PHVIDEODECODER_H
