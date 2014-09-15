#include <QSerialPortInfo>

#include "SerialTestWindow.h"
#include "ui_SerialTestWindow.h"
#include "PhTools/PhDebug.h"

SerialTestWindow::SerialTestWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SerialTestWindow),
	_serialA(this),
	_serialB(this)
{
	ui->setupUi(this);

	connect(ui->sendButton1, &QPushButton::clicked, this, &SerialTestWindow::sendTextA);
	connect(&_serialA, &QSerialPort::readyRead, this, &SerialTestWindow::readTextA);
	if(open(&_serialA, "A"))
		_serialA.write("Hello from serial A");


	connect(ui->sendButton2, &QPushButton::clicked, this, &SerialTestWindow::sendTextB);
	connect(&_serialB, &QSerialPort::readyRead, this, &SerialTestWindow::readTextB);
	if(open(&_serialB, "B"))
		_serialB.write("Hello from serial B");

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
	_serialA.write(ui->inputA->text().toUtf8().constData());
}

void SerialTestWindow::sendTextB()
{
	_serialB.write(ui->inputB->text().toUtf8().constData());
}

void SerialTestWindow::readTextA()
{
	char buffer[256];
	qint64 n = _serialA.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	ui->receiveA->setText(ui->receiveA->toPlainText() + s);

	checkCTS();
}

void SerialTestWindow::readTextB()
{
	char buffer[256];
	qint64 n = _serialB.read(buffer, 256);
	buffer[n] = 0;
	QString s(buffer);
	ui->receiveB->setText(ui->receiveB->toPlainText() + s);

	checkCTS();
}

void SerialTestWindow::on_checkA_toggled(bool checked)
{
	if(checked)
		open(&_serialA, "A");
	else
		closeA();
}

void SerialTestWindow::on_checkB_toggled(bool checked)
{
	if(checked)
		open(&_serialB, "B");
	else
		closeB();
}

bool SerialTestWindow::open(QSerialPort * serial, QString suffix)
{
	PHDEBUG << "open" << suffix;

	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
		QString name = info.portName();
		if(name.startsWith("usbserial-")) {
			if(name.endsWith(suffix)) {
				serial->setPort(info);
				serial->setBaudRate(QSerialPort::Baud38400);
				serial->setDataBits(QSerialPort::Data8);
				serial->setStopBits(QSerialPort::OneStop);
				serial->setParity(QSerialPort::OddParity);

				PHDEBUG << "Opening " << name;
				serial->open(QSerialPort::ReadWrite);

				return true;
			}
		}
	}
	PHDEBUG << "not found";
	return false;
}

void SerialTestWindow::closeA()
{
	PHDEBUG << "Closing " << _serialA.objectName();
	_serialA.close();
}

void SerialTestWindow::closeB()
{
	PHDEBUG << "Closing " << _serialB.objectName();
	_serialB.close();
}

void SerialTestWindow::checkCTS()
{
	bool cts = _serialA.pinoutSignals() & QSerialPort::ClearToSendSignal;
	float frequency = _ctsCounter.frequency();
	if(cts != _lastCTS) {
		_ctsCounter.tick();
		ui->ctsLabel->setText("CTS : " + QString::number(frequency));
		_lastCTS = cts;
	}

	_timerCounter.tick();
	PHDEBUG << _timerCounter.frequency() << frequency << cts;
}
