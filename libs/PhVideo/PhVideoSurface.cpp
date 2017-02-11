#include "PhVideoSurface.h"

PhVideoSurface::PhVideoSurface()
	: m_surface(NULL),
	  _currentFrame(PHFRAMEMIN),
	  _offset(0)
{
	m_format = QVideoSurfaceFormat();
}

QAbstractVideoSurface* PhVideoSurface::videoSurface() const
{
	return m_surface;
}

void PhVideoSurface::setVideoSurface(QAbstractVideoSurface *surface)
{
	if (m_surface != surface && m_surface && m_surface->isActive()) {
		m_surface->stop();
	}

	m_surface = surface;

	if (m_surface && m_format.isValid())
		m_surface->start(m_format);
}

void PhVideoSurface::update(PhVideoBuffer *buffer)
{
	_currentFrame = buffer->frame();

	QVideoFrame *videoFrame = buffer->videoFrame();

	// should set the format
	if ((m_format.frameSize() != videoFrame->size())
			|| (m_format.pixelFormat() != videoFrame->pixelFormat())) {
		m_format = QVideoSurfaceFormat(videoFrame->size(), videoFrame->pixelFormat());

		if (m_surface && m_format.isValid())
			m_surface->start(m_format);
	}

	if (m_surface) {
		m_surface->present(*videoFrame);
	}
}

PhFrame PhVideoSurface::currentFrame() const
{
	return _currentFrame;
}

void PhVideoSurface::discard()
{
	_currentFrame = PHFRAMEMIN;
}

PhFrame PhVideoSurface::offset() const
{
	return _offset;
}

void PhVideoSurface::setOffset(const PhFrame &offset)
{
	_offset = offset;
}
