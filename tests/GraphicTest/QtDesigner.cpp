#include "QtDesigner.h"
#include "ui_QtDesigner.h"


QtDesigner::QtDesigner(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QtDesigner)
{
	ui->setupUi(this);
	ui->Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	playButtonState = true;
	connect(ui->Play, SIGNAL(clicked()), this, SLOT(pushPlayButton()));
	ui->FastBackward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
	ui->FastForward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

}

QtDesigner::~QtDesigner()
{
	delete ui;
}

void QtDesigner::pushPlayButton()
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

