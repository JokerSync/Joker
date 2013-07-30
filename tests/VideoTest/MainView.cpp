#include "MainView.h"

#include <QtWidgets>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>

MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , playButton(0)
    , positionSlider(0)
    , errorLabel(0)
{
    // Adding buttons and slider
    QAbstractButton *openButton = new QPushButton(tr("Open..."));
    connect(openButton, SIGNAL(clicked()), this, SLOT(onOpenFile()));

    playButton = new QPushButton;
    playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playButton, SIGNAL(clicked()), &_videoController, SLOT(playPause()));

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 100);
    connect(positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderChanged(int)));

    errorLabel = new QLabel;
    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    // Create a first horizontal layout for buttons and slider
    QBoxLayout *controlLayout = new QHBoxLayout;
    //controlLayout->setMargin(0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    // Create a second vertical layout for the video view and the first layout
    QBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(_videoController.getView());
    layout->addLayout(controlLayout);
    layout->addWidget(errorLabel);

    // Add the layout to the main window
    this->setLayout(layout);

    // Set the slot for the video controller
    connect(&_videoController, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(&_videoController, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&_videoController, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(&_videoController, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));
}

bool MainView::openFile(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
        _videoController.open(fileName);
        playButton->setEnabled(true);
        return true;
    }
    return false;
}

void MainView::onOpenFile()
{
    errorLabel->setText("");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    openFile(fileName); // TODO: show error in case of error
}

void MainView::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void MainView::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
}

void MainView::durationChanged(qint64 duration)
{
//    positionSlider->setRange(0, duration);
}

void MainView::sliderChanged(int sliderPosition)
{
    _videoController.setPosition(_videoController.duration() * sliderPosition / 100);
}

void MainView::handleError()
{
    playButton->setEnabled(false);
    errorLabel->setText("Error: " + _videoController.errorString());
}
