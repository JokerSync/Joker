#include <QSerialPortInfo>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PhTools/PhDebug.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_serialA(this),
	_serialB(this)
{
	ui->setupUi(this);

	connect(ui->sendButton1, SIGNAL(clicked()), this, SLOT(sendTextA()));
	connect(&_serialA, SIGNAL(readyRead()), this, SLOT(readTextA()));
	if(open(&_serialA, "A"))
		_serialA.write("Hello from serial A");


	connect(ui->sendButton2, SIGNAL(clicked()), this, SLOT(sendTextB()));
	connect(&_serialB, SIGNAL(readyRead()), this, SLOT(readTextB()));
	if(open(&_serialB, "B"))
		_serialB.write("Hello from serial B");

	connect(&_ctsTimer, SIGNAL(timeout()), this, SLOT(checkCTS()));
	_ctsTimer.start(5);
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

	checkCTS();
}

void MainWindow::readTextB()
{
	char buffer[256];
	qint64 n = _serialB.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	ui->receiveB->setText(ui->receiveB->toPlainText() + s);

	checkCTS();
}

void MainWindow::on_checkA_toggled(bool checked)
{
	if(checked)
		open(&_serialA, "A");
	else
		closeA();
}

void MainWindow::on_checkB_toggled(bool checked)
{
	if(checked)
		open(&_serialB, "B");
	else
		closeB();
}

bool MainWindow::open(QSerialPort * serial, QString suffix)
{
	qDebug() << "open" << suffix;

	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts())
	{
		QString name = info.portName();
		if(name.startsWith("usbserial-"))
		{
			if(name.endsWith(suffix))
			{
				serial->setPort(info);
				serial->setBaudRate(QSerialPort::Baud38400);
				serial->setDataBits(QSerialPort::Data8);
				serial->setStopBits(QSerialPort::OneStop);
				serial->setParity(QSerialPort::OddParity);

				qDebug() << "Opening " << name;
				serial->open(QSerialPort::ReadWrite);

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

void MainWindow::closeB()
{
	qDebug() << "Closing " << _serialB.objectName();
	_serialB.close();
}

void MainWindow::checkCTS()
{
	bool cts = _serialA.pinoutSignals() & QSerialPort::ClearToSendSignal;
	float frequency = _ctsCounter.frequency();
	if(cts != _lastCTS)
	{
		_ctsCounter.tick();
		ui->ctsLabel->setText("CTS : " + QString::number(frequency));
		_lastCTS = cts;
	}

	_timerCounter.tick();
	PHDEBUG << _timerCounter.frequency() << frequency ;
}
