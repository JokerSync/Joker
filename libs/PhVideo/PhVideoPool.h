#ifndef PHVIDEOPOOL_H
#define PHVIDEOPOOL_H

#include <QObject>
#include <QMap>

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
	 * @return A video buffer, or NULL if there is none for that frame number
	 */
	PhVideoBuffer *tryGetFrame(PhFrame frame);

	/**
	 * @brief Pool of decoded buffers
	 * @return A read-only map of buffers
	 */
	const QMap<PhFrame, PhVideoBuffer *> decoded();

	/**
	 * @brief Update the pool information
	 * @param frameLength Video length
	 */
	void update(PhFrame frameLength);
signals:
	/**
	 * @brief Signal sent when a frame can be recycled
	 * @param buffer the frame to recycle
	 */
	void recycledFrame(PhVideoBuffer *buffer);

	/**
	 * @brief Signal sent when the decoding should stop
	 */
	void stop();

	/**
	 * @brief signal sent when the strip time has changed
	 * @param stripFrame the new strip frame
	 * @param backward true if the strip is being played backward
	 * @param stripFrameIsInPool true if the frame was found in the decoded frame pool
	 */
	void poolTimeChanged(PhFrame stripFrame, bool backward, bool stripFrameIsInPool);

public slots:
	/**
	 * @brief Handle a change in strip time
	 * @param stripFrame Starting frame
	 * @param backward True if playing backward, false if playing forward
	 */
	void stripTimeChanged(PhFrame stripFrame, bool backward);

	/**
	 * @brief Handle a frame that has just been decoded
	 * @param buffer the decoded buffer
	 */
	void frameAvailable(PhVideoBuffer *buffer);
private:
	PhVideoSettings *_settings;
	PhFrame _frameLength;
	QMap<PhFrame, PhVideoBuffer*> _decodedPool;
	PhFrame clip(PhFrame frame);
	bool _backward;
	PhFrame _stripFrame;
};

#endif // PHVIDEOPOOL_H
