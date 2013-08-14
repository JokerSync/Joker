#include "PhSonyController.h"

#include <QDebug>
#include <QSerialPortInfo>

PhSonyController::PhSonyController(QString comSuffix, QObject *parent) :
	QObject(parent), _comSuffix(comSuffix)
{
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
			_serial.setBaudRate(QSerialPort::Baud38400);
			_serial.setDataBits(QSerialPort::Data8);
			_serial.setStopBits(QSerialPort::OneStop);
			_serial.setParity(QSerialPort::OddParity);
			// TODO : check no more parameters are needed.
			qDebug() << "Opening " << name;
			return _serial.open(QSerialPort::ReadWrite);
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

void PhSonyController::test()
{
	if(_serial.isOpen())
	{
		qDebug() << "writing " << _serial.write("pouet") << " bytes.";
	}
}
