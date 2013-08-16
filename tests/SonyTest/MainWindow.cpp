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

	if(_sonyMaster.start())
	{
		qDebug() << "master open ok";

		if(_sonySlave.start())
		{
			qDebug() << "slave open ok";
			_sonyMaster.deviceTypeRequest();
		}
		else
			qDebug() << "error opening master";
	}
	else
		qDebug() << "error opening master";

}

MainWindow::~MainWindow()
{
	_sonyMaster.stop();
	_sonySlave.stop();
	delete ui;
}
