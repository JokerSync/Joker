/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhSonyController.h"

#include <QSerialPortInfo>
#include <qmath.h>

#include "PhTools/PhDebug.h"

PhSonyController::PhSonyController(PhSonySettings *settings, QString comSuffix) :
	_settings(settings),
	_comSuffix(comSuffix),
	_dataRead(0),
	_lastCTS(false),
	_threadRunning(false)
{
//	connect(&_serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
//            SLOT(handleError(QSerialPort::SerialPortError)));
}

PhSonyController::~PhSonyController()
{
	close();
}

bool PhSonyController::open(bool inThread)
{
	PHDEBUG << _comSuffix;
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
		QString name = info.portName();
		PHDEBUG << name;

		if(name.endsWith(_comSuffix)) {
			_serial.setPort(info);

			PHDEBUG << _comSuffix << "Opening " << name << _serial.parent();
			if( _serial.open(QSerialPort::ReadWrite)) {
				_serial.setBaudRate(QSerialPort::Baud38400);
				_serial.setDataBits(QSerialPort::Data8);
				_serial.setStopBits(QSerialPort::OneStop);
				_serial.setParity(QSerialPort::OddParity);

				if(inThread)
					this->start(QThread::HighPriority);
				else
					connect(&_serial, SIGNAL(readyRead()), this, SLOT(onData()));
				return true;
			}
		}
	}
	PHDEBUG << _comSuffix << "Unable to find usbserial-XXX" << _comSuffix;
	return false;
}

void PhSonyController::close()
{
	if(_threadRunning) {
		_threadRunning = false;
		PHDEBUG << this->wait(1000);
	}
	if(_serial.isOpen()) {
		PHDEBUG << _comSuffix;
		_serial.close();
	}
}

void PhSonyController::checkVideoSync()
{
	if(_serial.isOpen()) {
		bool videoSyncUp = true;
		if(_settings)
			videoSyncUp = _settings->videoSyncUp();
		bool cts = _serial.pinoutSignals() & QSerialPort::ClearToSendSignal;
		if(videoSyncUp) {
			if(!_lastCTS && cts) {
				PHDBG(24);
				onVideoSync();
				emit videoSync();
			}
		}
		else {
			if(_lastCTS && !cts) {
				PHDBG(24);
				onVideoSync();
				emit videoSync();
			}
		}
		_lastCTS = cts;
	}
}

void PhSonyController::run()
{
	_threadRunning = true;
	while(_threadRunning) {
		this->checkVideoSync();
		if(_serial.waitForReadyRead(10))
			onData();
	}
}

PhRate PhSonyController::computeRate(unsigned char data1)
{
	if(data1 == 0)
		return 0;
	PhRate n1 = data1;
	return qPow(10, n1/32 - 2);
}

PhRate PhSonyController::computeRate(unsigned char data1, unsigned char data2)
{
	PhRate n1 = data1;
	PhRate n2 = data2;
	PhRate rate = computeRate(data1);
	return rate + n2/256 * qPow(10, (n1+1)/32 - 2 - rate);
}

unsigned char PhSonyController::computeData1FromRate(PhRate rate)
{
	if(rate == 0)
		return 0;
	return (char)(32 * (2 + qLn(rate) / qLn(10)));
}

unsigned char PhSonyController::getDataSize(unsigned char cmd1)
{
	return cmd1 & 0x0f;
}

void PhSonyController::sendCommandWithData(unsigned char cmd1, unsigned char cmd2, const unsigned char *data)
{
//	PHDEBUG << _comSuffix << stringFromCommand(cmd1, cmd2, data);
	unsigned char datacount = getDataSize(cmd1);
	unsigned char checksum = cmd1 + cmd2;
	for (int i = 0; i < datacount; i++) {
		_dataOut[i + 2] = data[i];
		checksum += data[i];
	}
	_dataOut[0] = cmd1;
	_dataOut[1] = cmd2;
	_dataOut[datacount+2] = checksum;
	_serial.write((const char*)_dataOut, datacount + 3);
}

void PhSonyController::sendCommand(unsigned char cmd1, unsigned char cmd2, ...)
{
	unsigned char data[256];
	unsigned char datacount = getDataSize(cmd1);
	va_list argumentList;
	va_start(argumentList, cmd2);
	for (int i = 0; i < datacount; i++)
		data[i] = (char)va_arg(argumentList, int);

	va_end(argumentList);

	sendCommandWithData(cmd1, cmd2, data);
}

void PhSonyController::timeOut()
{
	PHDEBUG << _comSuffix;
}

void PhSonyController::checkSumError()
{
	PHDEBUG << _comSuffix;
}

QString PhSonyController::stringFromCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char * data)
{
	QString dataString = "";
	unsigned char dataCount = getDataSize(cmd1);
	if(dataCount > 0) {
		dataString = " : ";
		for(int i = 0; i < dataCount; i++)
			dataString += " " + QString::number(data[i], 16);
	}
	return QString::number(cmd1, 16) + " " + QString::number(cmd2, 16) + dataString;
}

void PhSonyController::onData()
{
	while(_serial.bytesAvailable()) {
		//	PHDEBUG << _comSuffix;
		// reading the cmd1 and cmd2
		if(_dataRead < 2) {
			QByteArray array = _serial.read(2 - _dataRead);
			//		PHDEBUG << "reading : " << array.length();
			for (int i = 0; i < array.length(); i++)
				_dataIn[i + _dataRead] = array[i];
			_dataRead += array.length();
		}

		// if cmd1 and cmd2 are read, go on with data
		if(_dataRead >= 2) {
			unsigned char cmd1 = _dataIn[0];
			unsigned char cmd2 = _dataIn[1];
			unsigned char datacount = getDataSize(cmd1);

			// Reading the data left
			QByteArray array = _serial.read(datacount + 3 - _dataRead);
			//		PHDEBUG << "reading : " << array.length();
			for (int i = 0; i < array.length(); i++)
				_dataIn[i + _dataRead] = array[i];
			_dataRead += array.length();

			if(_dataRead == datacount + 3) { // A whole command has been read
				QString cmdString = stringFromCommand(cmd1, cmd2, _dataIn + 2);
				//			PHDEBUG << _comSuffix << "reading : " << cmdString;

				// Computing the checksum
				unsigned char checksum = 0;
				for (int i = 0; i < datacount + 2; i++)
					checksum += _dataIn[i];

				if (checksum != _dataIn[datacount+2]) {
					PHDEBUG << _comSuffix << "Checksum error : " << cmdString;
					_serial.flush();
					checkSumError();
				}
				else // Process the data
					processCommand(cmd1, cmd2, _dataIn + 2);

				// Reset the data counter to read another command
				_dataRead = 0;
			}
		}
	}
}

void PhSonyController::handleError(QSerialPort::SerialPortError error)
{
	PHDEBUG << _comSuffix << error;
}
