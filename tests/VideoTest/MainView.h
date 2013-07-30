
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
    MainView(QWidget *parent = 0);
    bool openFile(QString fileName);

public slots:
    void onOpenFile();

private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void sliderChanged(int sliderPosition);
    void handleError();

private:
    PhVideoController _videoController;
    QAbstractButton *playButton;
    QSlider *positionSlider;
    QLabel *errorLabel;
};

#endif
