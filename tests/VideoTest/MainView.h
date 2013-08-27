
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <qmediaplayer.h>

#include <QString>
#include <QtGui/QMovie>
#include <QtWidgets/QWidget>

#include <QTimer>

#include "PhTools/PhClock.h"

//#define VLC

#ifdef VLC
#include "PhVideo/PhVLCVideoView.h"
#else
#include "PhVideo/PhVideoView.h"
#endif

using namespace std;

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
QT_END_NAMESPACE

class MainView : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief MainView constructor
     */
    MainView();
    /**
     * Open a video file.
     * @param fileName Path of the video file to open.
     * @return True if succeeds, false otherwise.
     */
    bool openFile(QString fileName);

public slots:


private slots:

	void onOpenFile();

private:
#ifdef VLC
	PhVLCVideoView _videoView;
#else
	PhVideoView _videoView;
#endif
    QAbstractButton *_openButton;
	PhClock _clock;
};

#endif
