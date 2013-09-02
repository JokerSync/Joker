#include <QDebug>
#include <QSerialPortInfo>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_serialA(this),
	_serialB(this)
{
	ui->setupUi(this);

	connect(ui->sendButton1, SIGNAL(clicked()), this, SLOT(sendTextA()));
	connect(&_serialA, SIGNAL(readyRead()), this, SLOT(readTextA()));
	openA();

	connect(ui->sendButton2, SIGNAL(clicked()), this, SLOT(sendTextB()));
	connect(&_serialB, SIGNAL(readyRead()), this, SLOT(readTextB()));
	openB();
}

MainWindow::~MainWindow()
{
	closeA();
	closeB();
	delete ui;
}

void MainWindow::sendTextA()
{
	_serialA.write(ui->inputA->text().toUtf8().constData());
}

void MainWindow::sendTextB()
{
	_serialB.write(ui->inputB->text().toUtf8().constData());
}

void MainWindow::readTextA()
{
	char buffer[256];
	qint64 n = _serialA.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	ui->receiveA->setText(ui->receiveA->toPlainText() + s);
}

void MainWindow::readTextB()
{
	char buffer[256];
	qint64 n = _serialB.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	ui->receiveB->setText(ui->receiveB->toPlainText() + s);
}

void MainWindow::on_checkA_toggled(bool checked)
{
	if(checked)
		openA();
	else
		closeA();
}

void MainWindow::on_checkB_toggled(bool checked)
{
	if(checked)
		openB();
	else
		closeB();
}

bool MainWindow::openA()
{
	qDebug() << "openA";
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts())
	{
		QString name = info.portName();
		if(name.startsWith("usbserial-"))
		{
			if(name.endsWith("A"))
			{
				_serialA.setPort(info);
				qDebug() << "Opening " << name;
				_serialA.open(QSerialPort::ReadWrite);

				connect(ui->sendButton1, SIGNAL(clicked()), this, SLOT(sendTextA()));
				connect(&_serialA, SIGNAL(readyRead()), this, SLOT(readTextA()));

				_serialA.write("Hello from serial 1!");

				return true;
			}
		}
	}
	qDebug() << "not found";
	return false;
}

void MainWindow::closeA()
{
	qDebug() << "Closing " << _serialA.objectName();
	_serialA.close();
}

bool MainWindow::openB()
{
	qDebug() << "openB";
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts())
	{
		QString name = info.portName();
		if(name.startsWith("usbserial-"))
		{
			if(name.endsWith("B"))
			{
				_serialB.setPort(info);
				qDebug() << "Opening " << name;
				_serialB.open(QSerialPort::ReadWrite);

				connect(ui->sendButton2, SIGNAL(clicked()), this, SLOT(sendTextB()));
				connect(&_serialB, SIGNAL(readyRead()), this, SLOT(readTextB()));

				_serialB.write("Hello from serial 2!");

				return true;
			}
		}
	}
	qDebug() << "not found";
	return false;
}

void MainWindow::closeB()
{
	qDebug() << "Closing " << _serialB.objectName();
	_serialB.close();
}
