/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhSonyController.h"

#include <qmath.h>

#include "PhTools/PhDebug.h"

PhSonyController::PhSonyController(PhTimeCodeType tcType, PhSyncSettings *settings, int deviceNumber) :
	_serial(0),
	_clock(tcType),
	_settings(settings),
	_deviceNumber(deviceNumber),
	_totalByteRead(0),
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

bool PhSonyController::open()
{
	PHDEBUG << "Opening" << _deviceNumber;
	FT_STATUS status = FT_Open(_deviceNumber, &_serial);
	if(status == FT_OK) {
		FT_SetBaudRate(_serial, FT_BAUD_38400);
		FT_SetDataCharacteristics(_serial, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_ODD);
		FT_SetTimeouts(_serial, 10, 10);
		this->start(QThread::HighPriority);
		return true;
	}

	// Get more info on connected device if failed
	DWORD deviceCount = 0;

	status = FT_CreateDeviceInfoList(&deviceCount);
	if(status != FT_OK) {
		PHDEBUG << "FT_CreateDeviceInfoList returned:" << status;
		return false;
	}

	if(deviceCount == 0) {
		PHDEBUG << "No connected device...";
		return false;
	}

	PHDEBUG << "Connected devices:" << deviceCount;

	FT_DEVICE_LIST_INFO_NODE *infos = new FT_DEVICE_LIST_INFO_NODE[deviceCount];
	status = FT_GetDeviceInfoList(infos, &deviceCount);
	if(status != FT_OK) {
		PHDEBUG << "FT_GetDeviceInfoList returned:" << status;
		return false;
	}

	for(int i = 0; i < deviceCount; i++)
		PHDEBUG << "\t" << i << ":\t" << infos[i].Description;

	PHDEBUG << _deviceNumber << "Unable to open" << _deviceNumber;
	return false;
}

void PhSonyController::close()
{
	if(_threadRunning) {
		_threadRunning = false;
		PHDEBUG << this->wait(1000);
	}
	if(_serial) {
		FT_Close(_serial);
		_serial = 0;
	}
}

void PhSonyController::checkVideoSync(int)
{
	if(_serial) {
		bool videoSyncUp = true;
		if(_settings)
			videoSyncUp = _settings->videoSyncUp();
		ULONG status = 0;
		FT_GetModemStatus(_serial, &status);
		bool cts = status & 0x10;
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
		onData();
		checkVideoSync(100);
	}
	PHDEBUG << _deviceNumber << "bye bye";
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
	//	PHDEBUG << _ftdiDescription << stringFromCommand(cmd1, cmd2, data);
	unsigned char dataCount = getDataSize(cmd1);
	unsigned char checksum = cmd1 + cmd2;
	for (int i = 0; i < dataCount; i++) {
		_dataOut[i + 2] = data[i];
		checksum += data[i];
	}
	_dataOut[0] = cmd1;
	_dataOut[1] = cmd2;
	_dataOut[dataCount+2] = checksum;
	DWORD written;
	FT_Write(_serial, _dataOut, dataCount + 3, &written);
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
	PHDEBUG << _deviceNumber;
}

void PhSonyController::checkSumError()
{
	PHDEBUG << _deviceNumber;
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
	DWORD byteRead = 0;

	// reading the cmd1 and cmd2
	if(_totalByteRead < 2) {
		FT_Read(_serial, _dataIn + _totalByteRead, 2 - _totalByteRead, &byteRead);
		//PHDEBUG << "reading : " << byteReturned
		_totalByteRead += byteRead;
	}

	// if cmd1 and cmd2 are read, go on with data
	if(_totalByteRead >= 2) {
		unsigned char cmd1 = _dataIn[0];
		unsigned char cmd2 = _dataIn[1];
		unsigned char datacount = getDataSize(cmd1);

		// Reading the data left
		FT_Read(_serial, _dataIn + _totalByteRead, datacount + 3 - _totalByteRead, &byteRead);
		//PHDEBUG << "reading : " << byteRead;
		_totalByteRead += byteRead;

		if(_totalByteRead == datacount + 3) { // A whole command has been read
			QString cmdString = stringFromCommand(cmd1, cmd2, _dataIn + 2);
			//			PHDEBUG << _ftdiDescription << "reading : " << cmdString;

			// Computing the checksum
			unsigned char checksum = 0;
			for (int i = 0; i < datacount + 2; i++)
				checksum += _dataIn[i];

			if (checksum != _dataIn[datacount+2]) {
				PHDEBUG << _deviceNumber << "Checksum error : " << cmdString;
				checkSumError();
			}
			else // Process the data
				processCommand(cmd1, cmd2, _dataIn + 2);

			// Reset the data counter to read another command
			_totalByteRead = 0;
		}
	}
}
