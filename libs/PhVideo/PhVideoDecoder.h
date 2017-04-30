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

#include <QObject>

#include "PhSync/PhTimeCode.h"
#include "PhVideoSettings.h"

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
	explicit PhVideoDecoder(PhVideoSettings *settings);

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
	 * @brief Signal sent when the deinterlace settings change
	 * @param deinterlace Whether the video should be deinterlaced
	 */
	void setDeinterlace(bool deinterlace);

	/**
	 * @brief inform the decoder that the strip time has changed
	 * @param stripFrame the new strip frame
	 * @param backward true if the strip is being played backward
	 * @param stripFrameIsInPool true if the frame was found in the decoded frame pool
	 */
	void stripTimeChanged(PhFrame stripFrame, bool backward, bool stripFrameIsInPool);

	/**
	 * @brief Stop
	 *
	 * Stop the decoder, prevents further decoding (including read-ahead)
	 */
	void stop();

	/**
	 * @brief transfer an unused frame buffer
	 * @param frame the unused frame buffer
	 */
	void recycleFrame(PhVideoBuffer *frame);
signals:
	/**
	 * @brief Signal sent when a frame has been decoded
	 * @param buffer The frame where the decoded frame is
	 */
	void frameAvailable(PhVideoBuffer *buffer);

	/**
	 * @brief Signal sent when the decoder is ready
	 * @param tcType The video file timecode type
	 * @param frameIn The number of the video file first frame
	 * @param frameLength The length of the video file
	 * @param width The width of the frame
	 * @param height The height of the frame
	 * @param codecName The codec name
	 */
	void opened(PhTimeCodeType tcType, PhFrame frameIn, PhFrame frameLength, int width, int height, QString codecName);

	/**
	 * @brief Signal sent when the decoder failed to open the file
	 */
	void openFailed();

private:
	bool ready();
	double framePerSecond();
	PhFrame frameLength();
	void frameToRgb(AVFrame *avFrame);
	int width();
	int height();
	QString codecName();
	PhFrame frameIn();
	void decodeFrame();

	int64_t PhFrame_to_AVTimestamp(PhFrame frame);
	PhFrame AVTimestamp_to_PhFrame(int64_t timestamp);

	QString _fileName;
	PhTimeCodeType _tcType;

	AVFormatContext * _formatContext;
	AVStream *_videoStream;
	AVFrame * _videoFrame;
	struct SwsContext * _swsContext;
	PhFrame _currentFrame;
	PhFrame _stripFrame;
	PhFrame _seekFrame;
	bool _backward;
	bool _processing;
	bool _seek;
	bool _fastSeek;
	int _readAheadCount;
	int _recycledCount;
	int _seekThreshold;

	bool _useAudio;
	AVStream *_audioStream;
	AVFrame * _audioFrame;

	bool _deinterlace;

	QList<PhVideoBuffer *> _recycledFrames;
	PhVideoBuffer *getAvailableFrame();
	PhFrame readAheadFrame();
	bool canDecode();
	PhFrame clip(PhFrame frame);
	void fastForwardToFrame(PhFrame frame);
};

#endif // PHVIDEODECODER_H
