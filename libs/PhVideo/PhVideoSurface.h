#ifndef PHVIDEOSURFACE_H
#define PHVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>

#include "PhVideoBuffer.h"

class PhVideoSurface : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface)

public:
	PhVideoSurface();

	QAbstractVideoSurface* videoSurface() const;

	void setVideoSurface(QAbstractVideoSurface *surface);

	/**
	 * @brief Update the video texture with the buffer content
	 * @param buffer A PhVideoBuffer containing RGB data
	 */
	void update(PhVideoBuffer *buffer);

	/**
	 * @brief Current frame number contained in the texture
	 * @return A frame number
	 */
	PhFrame currentFrame() const;

	/**
	 * @brief Discard the current frame number
	 */
	void discard();

	PhFrame offset() const;

	void setOffset(const PhFrame &offset);

private:
	QAbstractVideoSurface *m_surface;
	QVideoSurfaceFormat m_format;
	PhFrame _currentFrame;
	PhFrame _offset;
};

#endif // PHVIDEOSURFACE_H
