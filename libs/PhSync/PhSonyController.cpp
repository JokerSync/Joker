#include "PhSonyController.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <qmath.h>

PhSonyController::PhSonyController(QString comSuffix, QObject *parent) :
	QObject(parent), _comSuffix(comSuffix)
{
	connect(&_serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
	connect(&_serial, SIGNAL(readyRead()), this, SLOT(onData()));
	// TODO : handle CTS
	//connect(&_serial, SIGNAL(flowControlChanged(QSerialPort::FlowControl)), this, SLOT(onCTS(QSerialPort::FlowControl)));
}

PhSonyController::~PhSonyController()
{
	close();
}

bool PhSonyController::open()
{
	qDebug() << "PhSonyController::open()";
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts())
	{
		QString name = info.portName();
		if(name.startsWith("usbserial-") && name.endsWith(_comSuffix))
		{
			_serial.setPort(info);

			qDebug() << "Opening " << name;
			if( _serial.open(QSerialPort::ReadWrite))
			{
				_serial.setBaudRate(QSerialPort::Baud38400);
				_serial.setDataBits(QSerialPort::Data8);
				_serial.setStopBits(QSerialPort::OneStop);
				_serial.setParity(QSerialPort::OddParity);

				return true;
			}
		}
	}
	qDebug() << "Unable to find usbserial-XXX" << _comSuffix;
	return false;
}

void PhSonyController::close()
{
	qDebug() << "PhSonyController::close()";
	if(_serial.isOpen())
	{
		_serial.close();
	}
	else
		qDebug() << "port already closed.";
}

PhRate PhSonyController::computeRate(unsigned char data1)
{
	PhRate n1 = data1;
	return qPow(10, n1/32 - 2);
}

PhRate PhSonyController::computeRate(unsigned char data1, unsigned char data2)
{
	PhRate n1 = data1;
	PhRate n2 = data2;
	PhRate rate = this->computeRate(data1);
	return rate + n2/256 * qPow(10, (n1+1)/32 - 2 - rate);
}

unsigned char PhSonyController::computeData1(PhRate rate)
{
	return (unsigned char)(32 * (2 + qLn(rate) / qLn(10)));
}

unsigned char PhSonyController::status(int index)
{
	return _status[index];
}

unsigned char PhSonyController::getDataSize(unsigned char cmd1)
{
	return cmd1 & 0x0f;
}

void PhSonyController::sendCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data)
{
	QByteArray buffer;
	unsigned char datacount = getDataSize(cmd1);
	buffer[0] = cmd1;
	buffer[1] = cmd2;
	unsigned char checksum = cmd1 + cmd2;
	for (int i=0; i<datacount; i++)
	{
		buffer[i + 2] = data[i];
		checksum += data[i];
	}
	buffer[datacount+2] = checksum;
	_serial.write(buffer);
}

void PhSonyController::sendCommand(unsigned char cmd1, unsigned char cmd2, ...)
{
	unsigned char data[256];
	unsigned char datacount = getDataSize(cmd1);
	va_list argumentList;
	va_start(argumentList, cmd2);
	for (int i=0; i<datacount; i++)
		data[i] = (unsigned char)va_arg(argumentList, int);

	va_end(argumentList);

	sendCommand(cmd1, cmd2, data);
}

void PhSonyController::sendAck()
{
	qDebug() << "sendAck";
	sendCommand(0x10, 0x01);
}

void PhSonyController::sendNak(PhSonyController::PhSonyError error)
{
	qDebug() << "sendNak : " << error;
	sendCommand(0x11, 0x12, error);
}

QString PhSonyController::stringFromCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char * data)
{
	QString dataString = "";
	unsigned char dataCount = getDataSize(cmd1);
	if(dataCount > 0)
	{
		dataString = " : ";
		for(int i = 0; i < dataCount; i++)
			dataString += " " + QString::number(data[i], 16);
	}
	return QString::number(cmd1, 16) + " " + QString::number(cmd2, 16) + dataString;
}

void PhSonyController::onData()
{
	qDebug() << "onData";
	// read the serial data
	unsigned char buffer[256];
	int dataRead = 0;
	int nbTry = 0;

	// reading the cmd1 and cmd2
	while (dataRead < 2)
	{
		// TODO : check CTS
		QByteArray array = _serial.read(2);
		for (int i = 0; i< array.length(); i++)
			buffer[i + dataRead] = array[i];
		dataRead += array.length();
		nbTry++;
		if(nbTry > 200)
		{
			qDebug() << "Read time out";
			sendNak(PhSonyController::TimeOut);
			return;
		}
	}

	char cmd1 = buffer[0];
	char cmd2 = buffer[1];
	unsigned char datacount = getDataSize(cmd1);
	nbTry = 0;

	// Reading the data
	while (dataRead < datacount + 3)
	{
		// TODO : check CTS
		QByteArray array = _serial.read(datacount + 3 - dataRead);
		for (int i = 0; i< array.length(); i++)
			buffer[i + dataRead] = array[i];
		dataRead += array.length();
		nbTry++;
		if(nbTry > 200)
		{
			qDebug() << "Read time out";
			sendNak(PhSonyController::TimeOut);
			return;
		}
	}

	QString cmdString = stringFromCommand(cmd1, cmd2, buffer + 2);
	qDebug() << "reading : " << cmdString;

	// Computing the checksum
	unsigned char checksum = 0;
	for (int i=0; i < datacount + 2; i++)
		checksum += buffer[i];

	if (checksum != buffer[datacount+2])
	{
		qDebug() << "Checksum error : " << cmdString;
		_serial.flush();
		sendNak(PhSonyController::ChecksumError);
		return;
	}

	// process the data
	processCommand(cmd1, cmd2, buffer + 2);
}

void PhSonyController::onCTS()
{
	// TODO : check if we sync on up or down frame and tick the clock.
}

void PhSonyController::handleError(QSerialPort::SerialPortError error)
{
	qDebug() << "Serial port error : " << error;
}
