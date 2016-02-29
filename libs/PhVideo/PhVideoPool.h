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
	 */
	explicit PhVideoPool(PhVideoSettings *settings);

	/**
	 * @brief Cancel the whole pool
	 */
	void cancel();

	/**
	 * @brief Clean up the pool by discarding the frame outside of the pool window
	 * @param frame The middle frame window
	 */
	void cleanup(PhFrame frame);

	/**
	 * @brief Retrieve the decoded video buffer for a specific frame
	 * @param frame A frame number
	 * @return A video buffer
	 */
	PhVideoBuffer *decoded(PhFrame frame);

	/**
	 * @brief Pool of decoded buffers
	 * @return A read only list of buffers
	 */
	const QList<PhVideoBuffer *> decoded();

	/**
	 * @brief Update the pool information
	 * @param frameLength Video length
	 */
	void update(PhFrame frameLength);
signals:
	/**
	 * @brief Signal sent to ask the decoder to decode a video frame
	 * @param buffer the requested frame
	 */
	void decodeFrame(PhVideoBuffer *buffer);

	/**
	 * @brief Signal sent to cancel a frame request
	 * @param buffer The buffer describing the request
	 */
	void cancelFrameRequest(PhVideoBuffer *buffer);

public slots:
	/**
	 * @brief Request the frames starting a given time and further according to the readhead
	 *
	 * @param frame Starting frame
	 * @param backward True if playing backward, false if playing forward
	 */
	void requestFrames(PhFrame frame, bool backward);

	/**
	 * @brief Handle a frame that has just been decoded
	 * @param buffer the decoded buffer
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
	 * @param frame The current frame
	 * @return True if the frame has already been requested
	 */
	bool isFrameRequested(PhFrame frame);

	void requestFrame(PhFrame frame);

	PhVideoSettings *_settings;
	PhFrame _frameLength;
	QList<PhVideoBuffer*> _recycledPool;
	QList<PhVideoBuffer*> _requestedPool;
	QList<PhVideoBuffer*> _cancelledPool;
	QList<PhVideoBuffer*> _decodedPool;
};

#endif // PHVIDEOPOOL_H
