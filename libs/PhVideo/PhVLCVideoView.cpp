#include "PhVLCVideoView.h"

#include <QFile>
#include <QDebug>

PhVLCVideoView::PhVLCVideoView(QWidget *parent) :
	QWidget(parent)
{
	qDebug() << "Using VLC for video playback.";

	vlcPlayer = NULL;

    /* Initialize libVLC */
    vlcInstance = libvlc_new(0, NULL);

    /* Complain in case of broken installation */
    if (vlcInstance == NULL) {
        qDebug() << "Qt libVLC player: Could not init libVLC";
        exit(1);
	}
}

PhVLCVideoView::~PhVLCVideoView()
{
	/* Release libVLC instance on quit */
    if (vlcInstance)
        libvlc_release(vlcInstance);
}

bool PhVLCVideoView::open(QString fileName)
{
	if(QFile::exists(fileName))
	{
	    // Stop if something is playing
	    if (vlcPlayer && libvlc_media_player_is_playing(vlcPlayer))
		{
			/* stop the media player */
	        libvlc_media_player_stop(vlcPlayer);

			// release the media player
	        libvlc_media_player_release(vlcPlayer);

	        // Reset application values
	        vlcPlayer = NULL;
		}

	    // Create a new Media
	    libvlc_media_t *vlcMedia = libvlc_media_new_path(vlcInstance, fileName.toUtf8().constData());
	    if (!vlcMedia)
		{
			qDebug() << "Unable to open : "<< fileName;
	        return false;
		}

	    // Create a new libvlc player
	    vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);

	    // Release the media
	    libvlc_media_release(vlcMedia);

	    // Integrate the video in the interface
#if defined(Q_OS_MAC)
	    libvlc_media_player_set_nsobject(vlcPlayer, (void *)this->winId());
#elif defined(Q_OS_UNIX)
	    libvlc_media_player_set_xwindow(vlcPlayer, this->winId());
#elif defined(Q_OS_WIN)
	    libvlc_media_player_set_hwnd(vlcPlayer, this->winId());
#endif

	    // put the media in pause mode in order to display the first frame
	    libvlc_media_player_pause (vlcPlayer);

		return true;
	}
	else
	{
		qDebug() << "File does not exist: "<< fileName;
		return false;
	}
}

void PhVLCVideoView::setClock(PhClock *clock)
{
	PhVideoObject::setClock(clock);
	connect(_clock, SIGNAL(frameChanged()), this, SLOT(onFrameChanged()));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
}

void PhVLCVideoView::onRateChanged(PhRate rate)
{
	libvlc_media_player_play(vlcPlayer);
	libvlc_media_player_set_rate(vlcPlayer, rate);
}

void PhVLCVideoView::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	qDebug() << "PhVLCVideoView::onFrameChanged() : TODO";
}
