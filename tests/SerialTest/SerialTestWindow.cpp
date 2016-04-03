#include <QSerialPortInfo>

#include "SerialTestWindow.h"
#include "ui_SerialTestWindow.h"
#include "PhTools/PhDebug.h"

SerialTestWindow::SerialTestWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SerialTestWindow),
	_serialA(NULL),
	_serialB(NULL)
{
	ui->setupUi(this);

	connect(ui->sendButton1, &QPushButton::clicked, this, &SerialTestWindow::sendTextA);

	on_checkA_toggled(true);

	connect(ui->sendButton2, &QPushButton::clicked, this, &SerialTestWindow::sendTextB);
	_serialB = open("B");
	if(_serialB) {
		connect(_serialB, &QSerialPort::readyRead, this, &SerialTestWindow::readTextB);
		_serialB->write("Hello from serial B");
	}

	connect(&_ctsTimer, &QTimer::timeout, this, &SerialTestWindow::checkCTS);
	_ctsTimer.start(5);
}

SerialTestWindow::~SerialTestWindow()
{
	closeA();
	closeB();
	delete ui;
}

void SerialTestWindow::sendTextA()
{
	if(_serialA)
		_serialA->write(ui->inputA->text().toUtf8().constData());
}

void SerialTestWindow::sendTextB()
{
	if(_serialB)
		_serialB->write(ui->inputB->text().toUtf8().constData());
}

void SerialTestWindow::readTextA()
{
	if(_serialA) {
		char buffer[256];
		qint64 n = _serialA->read(buffer, 256);
		buffer[n] = 0;
		QString s(buffer);
		ui->receiveA->setText(ui->receiveA->toPlainText() + s);

		checkCTS();
	}
}

void SerialTestWindow::readTextB()
{
	if(_serialB) {
		char buffer[256];
		qint64 n = _serialB->read(buffer, 256);
		buffer[n] = 0;
		QString s(buffer);
		ui->receiveB->setText(ui->receiveB->toPlainText() + s);

		checkCTS();
	}
}

void SerialTestWindow::on_checkA_toggled(bool checked)
{
	if(checked) {
		_serialA = open("A");
		if(_serialA) {
			connect(_serialA, &QSerialPort::readyRead, this, &SerialTestWindow::readTextA);
			_serialA->write("Hello from serial A");
			return;
		}
	}
	closeA();
}

void SerialTestWindow::on_checkB_toggled(bool checked)
{
	if(checked) {
		_serialB = open("B");
		if(_serialB) {
			connect(_serialB, &QSerialPort::readyRead, this, &SerialTestWindow::readTextB);
			_serialB->write("Hello from serial B");
			return;
		}
	}
	closeB();
}

QSerialPort *SerialTestWindow::open(QString suffix)
{
	PHDEBUG << "open" << suffix;

	QSerialPort *serial = new QSerialPort(this);
	serial->setPortName(QString("cu.usbserial.%1").arg(suffix));
	serial->setBaudRate(QSerialPort::Baud38400);
	serial->setDataBits(QSerialPort::Data8);
	serial->setStopBits(QSerialPort::OneStop);
	serial->setParity(QSerialPort::OddParity);

	PHDEBUG << "Opening " << serial->portName();
	if(serial->open(QSerialPort::ReadWrite))
		return serial;
	delete serial;
	return NULL;
}

void SerialTestWindow::closeA()
{
	if(_serialA) {
		PHDEBUG << "Closing " << _serialA->portName();
		disconnect(_serialA, &QSerialPort::readyRead, this, &SerialTestWindow::readTextA);
		_serialA->close();
		_serialA = NULL;
	}
}

void SerialTestWindow::closeB()
{
	if(_serialB) {
		PHDEBUG << "Closing " << _serialB->portName();
		disconnect(_serialB, &QSerialPort::readyRead, this, &SerialTestWindow::readTextB);
		_serialB->close();
		_serialB = NULL;
	}
}

void SerialTestWindow::checkCTS()
{
	if(_serialA) {
		bool cts = _serialA->pinoutSignals() & QSerialPort::ClearToSendSignal;
		float frequency = _ctsCounter.frequency();
		if(cts != _lastCTS) {
			_ctsCounter.tick();
			ui->ctsLabel->setText("CTS : " + QString::number(frequency));
			_lastCTS = cts;
		}

		_timerCounter.tick();
		//	PHDEBUG << _timerCounter.frequency() << frequency << cts;
	}
}
