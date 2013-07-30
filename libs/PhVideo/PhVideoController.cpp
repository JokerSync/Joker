#include "PhVideoController.h"

PhVideoController::PhVideoController(QObject *parent) :
    QMediaPlayer(parent, QMediaPlayer::VideoSurface)
{
    this->setVideoOutput(&_videoView);
    connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(updatePercentageFromPosition(qint64)));
}

PhVideoView *PhVideoController::view()
{
    return &_videoView;
}

bool PhVideoController::open(QString fileName)
{
    this->setMedia(QUrl::fromLocalFile(fileName));
    return true;
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

void PhVideoController::updatePositionFromPercentage(int percentage)
{
    this->setPosition(percentage * this->duration() / 100);
}

void PhVideoController::updatePercentageFromPosition(qint64 position)
{
    if(this->duration() > 0)
        this->positionPercentageChanged(position * 100 / this->duration());
}


