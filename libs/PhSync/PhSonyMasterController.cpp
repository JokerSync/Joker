/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhSonyMasterController.h"

#include "PhTools/PhDebug.h"

PhSonyMasterController::PhSonyMasterController(PhTimeCodeType tcType, PhSyncSettings *settings)
	: PhSonyController(tcType, settings, settings->masterDeviceNumber())
{
}

void PhSonyMasterController::onVideoSync()
{
	timeSense();
	speedSense();
	statusSense();
}

void PhSonyMasterController::deviceTypeRequest()
{
	PHDEBUG << _deviceNumber << "Device type request";
	sendCommand(0x00, 0x11);
}

void PhSonyMasterController::play()
{
	PHDEBUG << _deviceNumber << "Play";
	sendCommand(0x20, 0x01);
}

void PhSonyMasterController::stop()
{
	PHDEBUG << _deviceNumber << "Stop";
	sendCommand(0x20, 0x00);
}

void PhSonyMasterController::cue(PhFrame frame)
{
	PhTimeCodeType tcType = _clock.timeCodeType();
	PHDEBUG << _deviceNumber << "Cue at " << PhTimeCode::stringFromFrame(frame, tcType);
	unsigned int bcd = PhTimeCode::bcdFromFrame(frame, tcType);
	sendCommandWithData(0x24, 0x31, (const unsigned char *)&bcd);
}

void PhSonyMasterController::fastForward()
{
	PHDEBUG << _deviceNumber;
	sendCommand(0x20, 0x10);
}

void PhSonyMasterController::rewind()
{
	PHDEBUG << _deviceNumber;
	sendCommand(0x20, 0x20);
}

void PhSonyMasterController::jog(PhRate rate)
{
	PHDEBUG << _deviceNumber;
	char data1;
	if (rate < 0) {
		data1 = computeData1FromRate(-rate);
		sendCommand(0x21, 0x21, data1);
	}
	else {
		data1 = computeData1FromRate(rate);
		sendCommand(0x21, 0x11, data1);
	}
}

void PhSonyMasterController::varispeed(PhRate rate)
{
	PHDEBUG << _deviceNumber << rate;
	char data1;
	if (rate < 0) {
		data1 = computeData1FromRate(-rate);
		sendCommand(0x21, 0x22, data1);
	}
	else {
		data1 = computeData1FromRate(rate);
		sendCommand(0x21, 0x12, data1);
	}
}

void PhSonyMasterController::shuttle(PhRate rate)
{
	PHDEBUG << _deviceNumber << rate;
	char data1;
	if (rate < 0) {
		data1 = computeData1FromRate(-rate);
		sendCommand(0x21, 0x23, data1);
	}
	else {
		data1 = computeData1FromRate(rate);
		sendCommand(0x21, 0x13, data1);
	}
}

void PhSonyMasterController::timeSense()
{
//	PHDEBUG << _ftdiDescription;
	sendCommand(0x61, 0x0c, 1);
}

void PhSonyMasterController::statusSense()
{
//	PHDEBUG << _ftdiDescription;
	sendCommand(0x61, 0x20, 4);
}

void PhSonyMasterController::speedSense()
{
//	PHDEBUG << _ftdiDescription;
	sendCommand(0x60, 0x2E);
}

void PhSonyMasterController::processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *dataIn)
{
//	PHDEBUG << _ftdiDescription << "PhSonyMasterController::processCommand : " << stringFromCommand(cmd1, cmd2, dataIn);
	switch (cmd1 >> 4) {
	case 1:
		switch (cmd2) {
		case 0x01:
			PHDEBUG << _deviceNumber << " => ACK";
			break;
		case 0x11:
			{
				deviceIdData(dataIn[0], dataIn[1]);
				QString id;
				id.sprintf("%02X %02X", dataIn[0], dataIn[1]);
				PHDEBUG << _deviceNumber << " => Device ID answer : " << id;
				break;
			}
		case 0x12:
			PHDEBUG << _deviceNumber << " => NAK :" <<  QString::number(dataIn[0], 16);
			break;
		default:
			PHDEBUG << _deviceNumber << " => Unknown answer : " << QString("%x %x").arg(cmd1, cmd2);
			break;
		}
		break;
	case 7:
		switch (cmd2) {
		case 0x04:
			{
				PhFrame frame = PhTimeCode::frameFromBcd(*(unsigned int *)dataIn, _clock.timeCodeType());
//			PHDEBUG << _ftdiDescription << " => LTC Time Data : " << PhTimeCode::stringFromFrame(frame, _clock.getTCType());
				_clock.setFrame(frame);
				break;
			}
		case 0x20:
			{
#warning /// @todo check more than 4 byte data.
				QString statusStr = "";
				for (int i = 0; i < 4; i++) {
					_status[i] = dataIn[i];
					statusStr += QString::number(dataIn[i], 16) + " ";
				}
				statusData(_status, 0, 4);
				//PHDEBUG << _ftdiDescription << " => Status data : " << statusStr;
				break;
			}
		case 0x2e:
			{
				unsigned dataCount = getDataSize(cmd1);
				PhRate rate = 0;
				switch(dataCount) {
				case 1:
					rate = computeRate(dataIn[0]);
					break;
				case 2:
					rate = computeRate(dataIn[0], dataIn[1]);
					break;
				default:
					PHDEBUG << _deviceNumber << " bad command";
					break;
				}

//			PHDEBUG << _ftdiDescription << " => Speed data : " << rate;
				_clock.setRate(rate);

				break;
			}
		default:
			PHDEBUG << _deviceNumber << " => Unknown answer : " << QString::number(cmd1, 16) << " " << QString::number(cmd2, 16);
			break;
		}
		break;
	default:
		PHDEBUG << _deviceNumber << " => Unknown answer : " << QString::number(cmd1, 16) << " " << QString::number(cmd2, 16);
		break;
	}
//	PHDEBUG << _ftdiDescription << stringFromCommand(cmd1, cmd2, dataIn) << " over";

}
