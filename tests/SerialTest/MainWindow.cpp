#include <QDebug>
#include <QSerialPortInfo>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_serial(this)
{
	ui->setupUi(this);

	qDebug() << "PhSonyController::open()";
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts())
	{
		QString name = info.portName();
		if(name.startsWith("usbserial-") && name.endsWith("A"))
		{
			_serial.setPort(info);
			qDebug() << "Opening " << name;
			_serial.open(QSerialPort::ReadWrite);

			connect(&_serial, SIGNAL(readyRead()), this, SLOT(readText()));

			_serial.write("Grand contrôle de mes tétons");
		}
	}


	connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendText()));
}

MainWindow::~MainWindow()
{
	qDebug() << "Closing " << _serial.objectName();
	_serial.close();
	delete ui;
}

void MainWindow::sendText()
{
	_serial.write(ui->lineEdit->text().toUtf8().constData());
}

void MainWindow::readText()
{
	char buffer[256];
	qint64 n = _serial.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	ui->textEdit->setText(ui->textEdit->toPlainText() + s);
}
