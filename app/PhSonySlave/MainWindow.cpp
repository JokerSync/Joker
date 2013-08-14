#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_sony(this)
{
	ui->setupUi(this);

	if(_sony.open())
	{
		qDebug() << "open ok";
		_sony.test();
	}
	else
		qDebug() << "error opening";
}

MainWindow::~MainWindow()
{
	delete ui;
}
