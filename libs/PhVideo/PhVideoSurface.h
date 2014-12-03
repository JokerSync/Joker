#ifndef PHVIDEOSURFACE_H
#define PHVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>

class PhVideoSurface : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface)

public:
	PhVideoSurface();

	QAbstractVideoSurface* videoSurface() const;

	void setVideoSurface(QAbstractVideoSurface *surface);

public slots:
	void onNewVideoContentReceived(const QVideoFrame &frame);

private:
	QAbstractVideoSurface *m_surface;
	QVideoSurfaceFormat m_format;
};

#endif // PHVIDEOSURFACE_H
