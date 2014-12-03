#include "PhVideoSurface.h"

PhVideoSurface::PhVideoSurface()
	: m_surface(NULL)
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

void PhVideoSurface::onNewVideoContentReceived(const QVideoFrame &frame)
{
	// should set the format
	if ((m_format.frameSize() != frame.size())
			|| (m_format.pixelFormat() != frame.pixelFormat()))
	{
		m_format = QVideoSurfaceFormat(frame.size(), frame.pixelFormat());

		if (m_surface && m_format.isValid())
			m_surface->start(m_format);
	}

	if (m_surface)
		m_surface->present(frame);
}
