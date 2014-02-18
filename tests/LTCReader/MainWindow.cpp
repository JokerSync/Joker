#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_reader(this)
{
	ui->setupUi(this);
	_clock = _reader.clock();
	ui->mediaController->setMediaLength(7500);
	ui->mediaController->setTCType(_clock->timeCodeType());
	ui->mediaController->setFirstFrame(_clock->frame());
	ui->mediaController->setClock(_clock);
	_reader.init();
}

MainWindow::~MainWindow()
{
	delete ui;
}
