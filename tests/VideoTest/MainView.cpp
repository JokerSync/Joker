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
	_framePerSecond = 25;

	_timer->start(40);
	_mediaControllerView = new PhMediaControllerView(_clock);
	qDebug() << "filesize:"<<_videoController.duration();
	qDebug() << "metadata:"<<_videoController.isMetaDataAvailable();



    // Add an open button
    _openButton = new QPushButton(tr("Open..."));
    // Open a file dialog when user click the open button
    connect(_openButton, SIGNAL(clicked()), this, SLOT(onOpenFile()));

	connect(_mediaControllerView, SIGNAL(useSliderCursorSignal()), this, SLOT(positionChanged()));

    // Update the video position when the user moves the slider
	//connect(_positionSlider, SIGNAL(sliderMoved(int)), &_videoController, SLOT(updatePositionFromPercentage(int)));
	connect(_clock, SIGNAL(rateChanged()), this, SLOT(changePlayerState()));
	connect(_mediaControllerView, SIGNAL(nextFrameButtonSignal()), this, SLOT(pushedNextFrameButton()));
	connect(_mediaControllerView, SIGNAL(previousFrameButtonSignal()), this, SLOT(pushedPreviousFrameButton()));
	connect(_mediaControllerView, SIGNAL(backButtonSignal()), this, SLOT(pushedBackButton()));



    // Add an error label
    // Update error label in case of video error
	//connect(&_videoController, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(updateErrorLabelContent()));

	connect(_clock, SIGNAL(rateChanged()), this, SLOT(changePlayerState()));

    // Create a first horizontal layout for buttons and slider
    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(5);
    controlLayout->addWidget(_openButton);

    // Create a second vertical layout for the video view and the first layout
    QBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
	layout->addWidget(_videoController.view());
    layout->addLayout(controlLayout);
	layout->addWidget(_mediaControllerView);

    // Add the layout to the main window
    this->setLayout(layout);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_videoController.play();
}

bool MainView::openFile(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
		_videoController.open(fileName);
		_videoController.setPlaybackRate(_clock->getRate());
		qDebug()<<"duration:"<<_videoController.duration();
        return true;
    }
	return false;
}


void MainView::updateFrame()
{
	int f;
	_clock->tick();
	f = _clock->getFrame();
	//_videoController.setPosition(f);
	qDebug()<<"frame"<<f<<"clock"<< 1000*f/24 <<"position"<< _videoController.position();

}

void MainView::changePlayerState()
{
	int r = _clock->getRate();

	if(r == 0)
	{
		_videoController.pause();
	}
	else
		_videoController.play();

	_videoController.setPlaybackRate(r);

	qDebug ()<<"rate"<<_videoController.playbackRate();
}

void MainView::positionChanged()
{
	qint64 f = _clock->getFrame();
	qint64 p = f*1000/_mediaControllerView->getFramePerSecond();
	_videoController.setPosition(p);
}

void MainView::pushedNextFrameButton()
{
	qint64 position = _videoController.position() + 1000/_framePerSecond;
	_videoController.setPosition(position);
	qDebug() << "metadata:"<<_videoController.isMetaDataAvailable();
}

void MainView::pushedPreviousFrameButton()
{
	qint64 position = _videoController.position() - 1000/_framePerSecond;
	_videoController.setPosition(position);
}

void MainView::pushedBackButton()
{
	_videoController.setPosition(0);
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


