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

	QObject::connect(ui->Text, SIGNAL(toggled(bool)),  ui->view, SLOT(displayText()));
	QObject::connect(ui->Image, SIGNAL(toggled(bool)), ui->view, SLOT(displayImage()));
	QObject::connect(ui->Rect, SIGNAL(toggled(bool)), ui->view, SLOT(displayRect()));


	QObject::connect(ui->Play, SIGNAL(clicked()), ui->view, SLOT(play()));
	QObject::connect(ui->FastBackward, SIGNAL(clicked()), ui->view, SLOT(fastBackward()));
	QObject::connect(ui->FastForward, SIGNAL(clicked()), ui->view, SLOT(fastForward()));

	QObject::connect(ui->TextBox, SIGNAL(textEdited(QString)), this, SLOT(changeText(QString)));

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

void MainWindow::changeText(QString)
{
	ui->view->getText()->setContent(ui->TextBox->text());
	ui->view->getText()->init();
}
