#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	playButtonState = true;
	connect(ui->Play, SIGNAL(clicked()), this, SLOT(pushPlayButton()));
	ui->FastBackward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
	ui->FastForward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

	_view = new GraphicTestView(this);
	_view->resize(800,400);

	QObject::connect(ui->Text, SIGNAL(toggled(bool)), _view, SLOT(displayText()));
	QObject::connect(ui->Image, SIGNAL(toggled(bool)), _view, SLOT(displayImage()));
	QObject::connect(ui->Rect, SIGNAL(toggled(bool)), _view, SLOT(displayRect()));


	QObject::connect(ui->Play, SIGNAL(clicked()), _view, SLOT(play()));
	QObject::connect(ui->FastBackward, SIGNAL(clicked()), _view, SLOT(fastBackward()));
	QObject::connect(ui->FastForward, SIGNAL(clicked()), _view, SLOT(fastForward()));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::pushPlayButton()
{
	if(playButtonState == true)
	{
		ui->Play->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		playButtonState = false;
	}
	else
	{
		ui->Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		playButtonState = true;
	}

}

