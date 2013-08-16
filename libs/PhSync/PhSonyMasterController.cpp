#include "PhSonyMasterController.h"

#include <QDebug>

PhSonyMasterController::PhSonyMasterController(QObject *parent)
	: PhSonyController("B", parent )
{
}

void PhSonyMasterController::deviceTypeRequest()
{
	qDebug() << "Device type request";
	sendCommand(0x00, 0x11);
}

void PhSonyMasterController::play()
{
	qDebug() << "Play";
	sendCommand(0x20, 0x01);
}

void PhSonyMasterController::stop()
{
	qDebug() << "Stop";
	sendCommand(0x20, 0x00);
}

void PhSonyMasterController::fastForward()
{
	qDebug() << "Fast forward";
	sendCommand(0x20, 0x10);
}

void PhSonyMasterController::rewind()
{
	qDebug() << "Rewind";
	sendCommand(0x20, 0x20);
}

void PhSonyMasterController::jog(PhRate rate)
{
	qDebug() << "Jog " << rate;
	unsigned char data1;
	if (rate < 0)
	{
		data1 = computeData1(-rate);
		sendCommand(0x21, 0x21, data1);
	}
	else
	{
		data1 = computeData1(rate);
		sendCommand(0x21, 0x11, data1);
	}
}

void PhSonyMasterController::varispeed(PhRate rate)
{
	qDebug() << "Varispeed " << rate;
	unsigned char data1;
	if (rate < 0)
	{
		data1 = computeData1(-rate);
		sendCommand(0x21, 0x22, data1);
	}
	else
	{
		data1 = computeData1(rate);
		sendCommand(0x21, 0x12, data1);
	}
}

void PhSonyMasterController::shuttle(PhRate rate)
{
	qDebug() << "Shuttle " << rate;
	unsigned char data1;
	if (rate < 0)
	{
		data1 = computeData1(-rate);
		sendCommand(0x21, 0x23, data1);
	}
	else
	{
		data1 = computeData1(rate);
		sendCommand(0x21, 0x13, data1);
	}
}

void PhSonyMasterController::timeSense()
{
	qDebug() << "Time sense";
	sendCommand(0x61, 0x0c, 1);
}

void PhSonyMasterController::statusSense()
{
	qDebug() << "Status sense";
	sendCommand(0x61, 0x20, 4);
}

void PhSonyMasterController::processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data)
{
	// TODO
}
