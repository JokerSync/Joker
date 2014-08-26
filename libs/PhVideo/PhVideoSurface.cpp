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

	QImage image(buffer->rgb(),
				 buffer->width(),
				 buffer->height(),
				 QImage::Format_RGB32);

	QVideoFrame *frame = new QVideoFrame(image);

	// should set the format
	if ((m_format.frameSize() != frame->size())
			|| (m_format.pixelFormat() != frame->pixelFormat()))
	{
		m_format = QVideoSurfaceFormat(frame->size(), frame->pixelFormat());

		if (m_surface && m_format.isValid())
			m_surface->start(m_format);
	}

	if (m_surface)
		m_surface->present(*frame);
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
