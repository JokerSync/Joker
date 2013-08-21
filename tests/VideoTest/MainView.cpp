#include "MainView.h"

#include <QtWidgets>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>

MainView::MainView()
	: QWidget(0),
	_positionSlider(0)
{
	_clock = new PhClock;
	_timer = new QTimer();
	_timer->start(40);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	qint64 filesize = _videoController.duration();




	_mediaControllerView = new PhMediaControllerView(_clock,PhTimeCodeType25, 2000, 850);
	qDebug() << "filesize"<<filesize;



    // Add an open button
    _openButton = new QPushButton(tr("Open..."));
    // Open a file dialog when user click the open button
    connect(_openButton, SIGNAL(clicked()), this, SLOT(onOpenFile()));


    // Play/pause the video when user click the play button
	//connect(_mediaControllerView, SIGNAL(playButtonSignal()), &_videoController, SLOT(playPause()));
    // Update the play button appearance when the video state change
	//connect(&_videoController, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(updatePlayButtonState(QMediaPlayer::State)));

    // Add a position slider
    _positionSlider = new QSlider(Qt::Horizontal);
	_positionSlider->setRange(0, 100);

    // Update the video position when the user moves the slider
	//connect(_positionSlider, SIGNAL(sliderMoved(int)), &_videoController, SLOT(updatePositionFromPercentage(int)));
	connect(_clock, SIGNAL(rateChanged()), this, SLOT(changePlayerState()));

	//Update the frame when user move the slider
	//connect(&_videoController, SIGNAL(positionPercentageChanged()),_clock, SLOT(_clock->setFrame(PhFrame)));
    // Update the slider position when the video position changes


    // Add an error label
    // Update error label in case of video error
	//connect(&_videoController, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(updateErrorLabelContent()));

	connect(_clock, SIGNAL(rateChanged()), this, SLOT(changePlayerState()));

    // Create a first horizontal layout for buttons and slider
    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(5);
    controlLayout->addWidget(_openButton);
	//controlLayout->addWidget(_positionSlider);

    // Create a second vertical layout for the video view and the first layout
    QBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
	layout->addWidget(_videoController.view());
    layout->addLayout(controlLayout);
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
		_videoController.setPlaybackRate(_clock->getRate());
        return true;
    }
	return false;
}


void MainView::updateFrame()
{
	//int f;
	_clock->tick();
	//f = _clock->getFrame();
	//_videoController.setPosition(f);
	//qDebug()<<"frame"<< f <<"position"<< _videoController.position();
}

void MainView::changePlayerState()
{
	int r = _clock->getRate();
	switch(r)
	{
	case 0:
		_videoController.pause();
		break;
	case 1:
		_videoController.play();
		break;
	case 4:
		_videoController.play();
		break;
	case -4:
		_videoController.play();
		break;

	}

	_videoController.setPlaybackRate(r);

	qDebug ()<<"rate"<<_videoController.playbackRate();
}

void MainView::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    openFile(fileName); // TODO: show error in case of error
}

/*void MainView::updatePlayButtonState(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}*/


