#include "PhVideoController.h"

PhVideoController::PhVideoController(QObject *parent) :
    QMediaPlayer(parent, QMediaPlayer::VideoSurface)
{
    this->setVideoOutput(&_videoView);
}

PhVideoView *PhVideoController::getView()
{
    return &_videoView;
}

bool PhVideoController::open(QString fileName)
{
    this->setMedia(QUrl::fromLocalFile(fileName));
}

void PhVideoController::playPause()
{
    switch(this->state())
    {
    case QMediaPlayer::PlayingState:
        this->pause();
        break;
    default:
        this->play();
        break;
    }
}


