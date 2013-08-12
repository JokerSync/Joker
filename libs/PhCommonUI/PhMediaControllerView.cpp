#include "PhMediaControllerView.h"
#include "ui_PhMediaControllerView.h"

PhMediaControllerView::PhMediaControllerView(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::PhMediaControllerView)
{
	ui->setupUi(this);

	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}

PhMediaControllerView::~PhMediaControllerView()
{
	delete ui;
}
