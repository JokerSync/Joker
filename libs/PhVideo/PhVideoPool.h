#ifndef PHVIDEOPOOL_H
#define PHVIDEOPOOL_H

#include <QObject>

#include "PhVideoSettings.h"
#include "PhVideoBuffer.h"

/**
 * @brief The video frame pool
 *
 * It is in charge of maintaining the frame pool, communcating with the decoder
 * through signal and slots
 */
class PhVideoPool : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief The PhVideoPool constructor
	 * @param settings The setting
	 * @param clock The engine clock
	 */
	explicit PhVideoPool(PhVideoSettings *settings, PhClock *clock);

	/**
	 * @brief Cancel the whole pool
	 */
	void cancel();

	/**
	 * @brief Clean up the pool by discarding the frame outside of the pool window
	 * @param time The middle time window
	 */
	void cleanup(PhTime time);

	/**
	 * @brief Retrieve the decoded video buffer for a specific time
	 * @param time A time value
	 * @return A video buffer
	 */
	PhVideoBuffer *decoded(PhTime time);

	/**
	 * @brief Pool of decoded buffers
	 * @return A read only list of buffers
	 */
	const QList<PhVideoBuffer *> decoded();

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
	 * @param buffer the requested frame
	 */
	void decodeFrame(PhVideoBuffer *buffer);

	/**
	 * @brief Signal sent to cancel a frame request
	 * @param frame The frame describing the request
	 */
	void cancelFrameRequest(PhVideoBuffer *buffer);

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
	void frameAvailable(PhVideoBuffer *buffer);

	/**
	 * @brief Handle the signal that a frame request has been cancelled in the decoder
	 * @param buffer The buffer describing the request
	 */
	void frameCancelled(PhVideoBuffer *buffer);
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
	QList<PhVideoBuffer*> _recycledPool;
	QList<PhVideoBuffer*> _requestedPool;
	QList<PhVideoBuffer*> _cancelledPool;
	QList<PhVideoBuffer*> _decodedPool;
};

#endif // PHVIDEOPOOL_H
