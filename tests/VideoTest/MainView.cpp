#include "MainView.h"

#include <QtWidgets>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>

MainView::MainView()
	: QWidget(0),
	  _videoView(this),
	_clock(this)
{
	_videoView.setClock(&_clock);
    // Add an open button
    _openButton = new QPushButton(tr("Open..."));
    // Open a file dialog when user click the open button
    connect(_openButton, SIGNAL(clicked()), this, SLOT(onOpenFile()));


    // Create a first horizontal layout for buttons and slider
    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(5);
    controlLayout->addWidget(_openButton);

    // Create a second vertical layout for the video view and the first layout
    QBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
	layout->addWidget(&_videoView);
    layout->addLayout(controlLayout);

    // Add the layout to the main window
    this->setLayout(layout);
}


bool MainView::openFile(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
		_videoView.open(fileName);
		_clock.setRate(1);
        return true;
    }
	return false;
}


void MainView::onOpenFile()
{
	 QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
	 openFile(fileName); // TODO: show error in case of error
}
