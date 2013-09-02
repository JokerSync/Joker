#include <QDebug>
#include <QSerialPortInfo>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_serial1(this),
	_serial2(this)
{
	ui->setupUi(this);

	qDebug() << "PhSonyController::open()";
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts())
	{
		QString name = info.portName();
		if(name.startsWith("usbserial-"))
		{
			if(name.endsWith("A"))
			{
				_serial1.setPort(info);
				qDebug() << "Opening " << name;
				_serial1.open(QSerialPort::ReadWrite);

				connect(ui->sendButton1, SIGNAL(clicked()), this, SLOT(sendText1()));
				connect(&_serial1, SIGNAL(readyRead()), this, SLOT(readText1()));

				_serial1.write("Hello from serial 1!");
			}
			if(name.endsWith("B"))
			{
				_serial2.setPort(info);
				qDebug() << "Opening " << name;
				_serial2.open(QSerialPort::ReadWrite);

				connect(ui->sendButton2, SIGNAL(clicked()), this, SLOT(sendText2()));
				connect(&_serial2, SIGNAL(readyRead()), this, SLOT(readText2()));

				_serial2.write("Hello from serial 2!");
			}
		}
	}

}

MainWindow::~MainWindow()
{
	qDebug() << "Closing " << _serial1.objectName();
	_serial1.close();
	qDebug() << "Closing " << _serial2.objectName();
	_serial2.close();
	delete ui;
}

void MainWindow::sendText1()
{
	_serial1.write(ui->input1->text().toUtf8().constData());
}

void MainWindow::sendText2()
{
	_serial2.write(ui->input2->text().toUtf8().constData());
}

void MainWindow::readText1()
{
	char buffer[256];
	qint64 n = _serial1.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	ui->textEdit1->setText(ui->textEdit1->toPlainText() + s);
}

void MainWindow::readText2()
{
	char buffer[256];
	qint64 n = _serial2.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	ui->textEdit2->setText(ui->textEdit2->toPlainText() + s);
}
