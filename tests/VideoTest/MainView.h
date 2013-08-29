
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QString>

#include "PhTools/PhClock.h"

#include "PhVideo/PhVideoView.h"

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
	PhVideoView _videoView;
    QAbstractButton *_openButton;
	PhClock _clock;
};

#endif
