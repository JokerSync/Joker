/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHAVDECODER_H
#define PHAVDECODER_H

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
#include <QMap>
#include <QSemaphore>
#include <QMutex>

#include "PhGraphic/PhGraphicTexturedRect.h"
#include "PhTools/PhTime.h"
#include "PhTools/PhTimeCode.h"

/**
 * @brief The PhAVDecoder class
 *
 * This class is the worker used to decode
 * the video file and store the frame in a
 * buffer
 */
class PhAVDecoder : public QObject
{
	Q_OBJECT
public:
	explicit PhAVDecoder(int bufferSize, QObject *parent = 0);
	~PhAVDecoder();

	/**
	 * @brief open a video file
	 * @param fileName the video file path
	 * @return True if succeed, false otherwise
	 */
	bool open(QString fileName);
	/**
	 * @brief close the decoder
	 *
	 * Close the AVStream, free the buffer
	 */
	void close();

	/**
	 * @brief timecode Type
	 * @return The timeCodeType(), processed
	 * with the video FPS
	 */
	PhTimeCodeType timeCodeType();
	/**
	 * @brief firstFrame
	 * @return The first frame of the video
	 */
	PhFrame firstFrame();
	/**
	 * @brief Set first frame
	 * @param frame the new first frame
	 */
	void setFirstFrame(PhFrame frame);

	/**
	 * @brief video width
	 * @return
	 */
	int width();
	/**
	 * @brief video height
	 * @return
	 */
	int height();
	/**
	 * @brief Get the length
	 * @return the length of the video
	 */
	PhFrame length();
	/**
	 * @brief get frame per second
	 * @return the FPS of the video file
	 */
	float framePerSecond();
	/**
	 * @brief Get the codec name
	 * @return the codec name
	 */
	QString codecName();

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
	 * @brief buffer occupation
	 * @return The buffer occupation
	 */
	int bufferOccupation();
	/**
	 * @brief getBuffer
	 *
	 * Give the pointer to the desired frame
	 * @param frame
	 * @return
	 */
	uint8_t* getBuffer(PhFrame frame);
signals:
	/**
	 * @brief finished
	 *
	 * For slot worker compatibility
	 */
	void finished();
	/**
	 * @brief error
	 * @param err
	 *
	 * For slot worker compatibility
	 */
	void error(QString err);

public slots:
	/**
	 * @brief process
	 *
	 * For slot worker compatibility
	 */
	void process();

	/**
	 * @brief onFrameChanged
	 * @param frame
	 *
	 * Handle the frame change
	 */
	void onFrameChanged(PhFrame frame, PhTimeCodeType);
	/**
	 * @brief onRateChanged
	 * @param rate
	 *
	 * Handle the rate change
	 */
	void onRateChanged(PhRate rate);

private:
	int64_t frame2time(PhFrame f);
	PhFrame time2frame(int64_t t);
	void decodeFrame(PhFrame frame);
	void clearBuffer();

	bool _interupted;
	int _bufferSize;
	QSemaphore _bufferFreeSpace;
	QMap<PhFrame, uint8_t * > _bufferMap;
	QMutex _bufferMutex;

	PhFrame _firstFrame;
	/**
	 * @brief The next frame that the decoder will process.
	 */
	PhFrame _nextDecodingFrame;
	PhFrame _lastDecodedFrame;
	PhFrame _oldFrame;
	int _direction;

	AVFormatContext * _pFormatContext;
	AVStream *_videoStream;
	AVFrame * _videoFrame;
	bool _deinterlace;

	AVStream *_audioStream;
	AVFrame * _audioFrame;
};

#endif // PHAVDECODER_H
