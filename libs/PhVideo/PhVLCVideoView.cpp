#include "PhVLCVideoView.h"

#include <QFile>
#include "PhTools/PhDebug.h"

PhVLCVideoView::PhVLCVideoView(QWidget *parent) :
	QWidget(parent)
{
	PHDEBUG << "Using VLC for video playback.";

	vlcPlayer = NULL;

    /* Initialize libVLC */
    vlcInstance = libvlc_new(0, NULL);

    /* Complain in case of broken installation */
    if (vlcInstance == NULL) {
        PHDEBUG << "Qt libVLC player: Could not init libVLC";
        exit(1);
	}

    connect(&_clock, SIGNAL(frameChanged()), this, SLOT(onFrameChanged(frame, PhTimeCodeType25)));
	connect(&_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
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
			PHDEBUG << "Unable to open : "<< fileName;
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
        libvlc_media_player_set_hwnd(vlcPlayer, (HWND)this->winId());
#endif

	    // put the media in pause mode in order to display the first frame
	    libvlc_media_player_pause (vlcPlayer);

		return true;
	}
	else
	{
		PHDEBUG << "File does not exist: "<< fileName;
		return false;
	}
}

void PhVLCVideoView::onRateChanged(PhRate rate)
{
	if(rate != 0)
	{
		libvlc_media_player_play(vlcPlayer);
		libvlc_media_player_set_rate(vlcPlayer, rate);
	}
	else
		libvlc_media_player_pause(vlcPlayer);
}

void PhVLCVideoView::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	libvlc_media_player_set_time(vlcPlayer, _clock.milliSecond());
}

void PhVLCVideoView::checkVideoPosition()
{
    //PhTime ms = libvlc_media_player_get_time(vlcPlayer);
    //PHDEBUG << ms;
	//_clock.setMillisecond(ms);
}
