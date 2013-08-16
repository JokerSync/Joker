#include "PhSonyController.h"

#include <QDebug>
#include <QSerialPortInfo>

PhSonyController::PhSonyController(QString comSuffix, QObject *parent) :
	QObject(parent), _comSuffix(comSuffix)
{
	connect(&_serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
	// TODO : handle onRead
	// TODO : handle CTS
}

PhSonyController::~PhSonyController()
{
	stop();
}


bool PhSonyController::start()
{
	qDebug() << "PhSonyController::open()";
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts())
	{
		QString name = info.portName();
		if(name.startsWith("usbserial-") && name.endsWith(_comSuffix))
		{
			_serial.setPort(info);
			_serial.setBaudRate(QSerialPort::Baud38400);
			_serial.setDataBits(QSerialPort::Data8);
			_serial.setStopBits(QSerialPort::OneStop);
			_serial.setParity(QSerialPort::OddParity);

			qDebug() << "Opening " << name;
			return _serial.open(QSerialPort::ReadWrite);
		}
	}
	qDebug() << "Unable to find usbserial-XXX" << _comSuffix;
	return false;
}

void PhSonyController::stop()
{
	qDebug() << "PhSonyController::close()";
	if(_serial.isOpen())
	{
		_serial.close();
	}
	else
		qDebug() << "port already closed.";
}

void PhSonyController::test()
{
	if(_serial.isOpen())
	{
		qDebug() << "writing " << _serial.write("pouet") << " bytes.";
	}
}

PhRate PhSonyController::computeRate(unsigned char data1)
{
	PhRate n1 = data1;
	return pow(10, n1/32 - 2);
}

PhRate PhSonyController::computeRate(unsigned char data1, unsigned char data2)
{
	PhRate n1 = data1;
	PhRate n2 = data2;
	PhRate rate = [self computeRateWithData1:data1];
	return rate + n2/256 * pow(10, (n1+1)/32 - 2 - rate);
}

unsigned char PhSonyController::computeData1(PhRate rate)
{
	return (unsigned char)(32*(2+log10(rate)));
}

unsigned char PhSonyController::status(int index)
{
	return status[index];
}

void PhSonyController::onData()
{
	// TODO : read the serial data and process it;
	//processCommand(cmd1, cmd2, data);
}

void PhSonyController::onCTS()
{
	// TODO : check if we sync on up or down frame and tick the clock.
}

void PhSonyController::handleError(QSerialPort::SerialPortError error)
{
	qDebug() << "Serial port error : " << error;
}
