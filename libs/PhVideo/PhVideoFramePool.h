#ifndef PHVIDEOFRAMEPOOL_H
#define PHVIDEOFRAMEPOOL_H

#include <QObject>

#include "PhVideoSettings.h"
#include "PhVideoFrame.h"

/**
 * @brief The video frame pool
 *
 * It is in charge of maintaining the frame pool, communcating with the decoder
 * through signal and slots
 */
class PhVideoFramePool : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief The PhVideoFramePool constructor
	 * @param settings The setting
	 * @param clock The engine clock
	 */
	explicit PhVideoFramePool(PhVideoSettings *settings, PhClock *clock);

	/**
	 * @brief Cancel the whole frame pool
	 */
	void cancel();

	/**
	 * @brief Clean up the pool by discarding the frame outside of the pool window
	 * @param time The middle time window
	 */
	void cleanup(PhTime time);

	/**
	 * @brief Retrieve the decoded video frame for a specific time
	 * @param time A time value
	 * @return A video frame
	 */
	PhVideoFrame *frame(PhTime time);

	/**
	 * @brief Pool of decoded frames
	 * @return A read only list of frames
	 */
	const QList<PhVideoFrame *> decodedFramePool();

	/**
	 * @brief Update the pool information
	 * @param timeIn Video time in
	 * @param length Video length
	 * @param tcType Timecode type
	 */
	void update(PhTime timeIn, PhTime length, PhTimeCodeType tcType);
signals:
	/**
	 * @brief Signal sent to ask the decoder to decode a video frame
	 * @param frame the requested frame
	 */
	void decodeFrame(PhVideoFrame *frame);

	/**
	 * @brief Signal sent to cancel a frame request
	 * @param frame The frame describing the request
	 */
	void cancelFrameRequest(PhVideoFrame *frame);

public slots:
	/**
	 * @brief Request the frames starting a given time and further according to the readhead
	 *
	 * @param time Starting time
	 */
	void requestFrames(PhTime time);

	/**
	 * @brief Handle a frame that has just been decoded
	 * @param frame the decoded frame
	 */
	void frameAvailable(PhVideoFrame *frame);

	/**
	 * @brief Handle the signal that a frame request has been cancelled in the decoder
	 * @param frame The frame describing the request
	 */
	void frameCancelled(PhVideoFrame *frame);
private:
	/**
	 * @brief Whether the time corresponds to the frame that we have requested
	 * @param time The current time
	 * @return True if the frame has already been requested
	 */
	bool isFrameRequested(PhTime time);

	void requestFrame(PhTime time);

	PhVideoSettings *_settings;
	PhClock *_clock;
	PhTime _timeIn, _length;
	PhTimeCodeType _tcType;
	QList<PhVideoFrame*> _recycledFramePool;
	QList<PhVideoFrame*> _requestedFramePool;
	QList<PhVideoFrame*> _cancelledFramePool;
	QList<PhVideoFrame*> _decodedFramePool;
};

#endif // PHVIDEOFRAMEPOOL_H
