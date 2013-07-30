
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <qmediaplayer.h>

#include <QString>
#include <QtGui/QMovie>
#include <QtWidgets/QWidget>

#include "PhVideo/PhVideoController.h"

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

private slots:
    void onOpenFile();
    void updatePlayButtonState(QMediaPlayer::State state);
    void updateErrorLabelContent();

private:
    PhVideoController _videoController;
    QAbstractButton *_playButton;
    QSlider *_positionSlider;
    QLabel *_errorLabel;
};

#endif
