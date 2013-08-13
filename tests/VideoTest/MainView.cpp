#include "MainView.h"

#include <QtWidgets>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>

MainView::MainView()
    : QWidget(0)
    , _playButton(0)
    , _positionSlider(0)
    , _errorLabel(0)
{
	_clock = new PhClock;
	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(40);


	_mediaControllerView = new PhMediaControllerView(_clock);
    // Add an open button
    _openButton = new QPushButton(tr("Open..."));
    // Open a file dialog when user click the open button
    connect(_openButton, SIGNAL(clicked()), this, SLOT(onOpenFile()));

    // Add a play button
	_playButton = new QPushButton;
    _playButton->setEnabled(false);
    _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    // Play/pause the video when user click the play button
	connect(_playButton, SIGNAL(clicked()), &_videoController, SLOT(playPause()));
    // Update the play button appearance when the video state change
    connect(&_videoController, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(updatePlayButtonState(QMediaPlayer::State)));

    // Add a position slider
    _positionSlider = new QSlider(Qt::Horizontal);
    _positionSlider->setRange(0, 100);
    // Update the video position when the user moves the slider
    connect(_positionSlider, SIGNAL(sliderMoved(int)), &_videoController, SLOT(updatePositionFromPercentage(int)));
    // Update the slider position when the video position changes
    connect(&_videoController, SIGNAL(positionPercentageChanged(int)), _positionSlider, SLOT(setValue(int)));

    // Add an error label
    _errorLabel = new QLabel;
    _errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    // Update error label in case of video error
    connect(&_videoController, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(updateErrorLabelContent()));

    // Create a first horizontal layout for buttons and slider
    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(5);
    controlLayout->addWidget(_openButton);
    controlLayout->addWidget(_playButton);
    controlLayout->addWidget(_positionSlider);

    // Create a second vertical layout for the video view and the first layout
    QBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(_videoController.view());
    layout->addLayout(controlLayout);
    layout->addWidget(_errorLabel);
	layout->addWidget(_mediaControllerView);

    // Add the layout to the main window
    this->setLayout(layout);
}

bool MainView::openFile(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
        _videoController.open(fileName);
        _playButton->setEnabled(true);
        _videoController.play();
        return true;
    }
	return false;
}

PhClock* MainView::get_clock()
{
	return _clock;
}

void MainView::updateFrame()
{
	_clock->tick();
}

void MainView::onOpenFile()
{
    _errorLabel->setText("");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    openFile(fileName); // TODO: show error in case of error
}

void MainView::updatePlayButtonState(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void MainView::updateErrorLabelContent()
{
    _playButton->setEnabled(false);
    _errorLabel->setText("Error: " + _videoController.errorString());
}
