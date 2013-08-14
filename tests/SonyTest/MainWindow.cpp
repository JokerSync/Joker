#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_sonyMaster(this),
	_sonySlave(this)
{
	ui->setupUi(this);

	if(_sonyMaster.open())
	{
		qDebug() << "master open ok";

		if(_sonySlave.open())
			qDebug() << "slave open ok";
		else
			qDebug() << "error opening master";
	}
	else
		qDebug() << "error opening master";

}

MainWindow::~MainWindow()
{
	delete ui;
}
