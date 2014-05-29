/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhSonySlaveController.h"

#include "PhTools/PhDebug.h"

PhSonySlaveController::PhSonySlaveController(PhTimeCodeType tcType, PhSyncSettings *settings)
	: PhSonyController(tcType, settings, settings->slaveDeviceNumber()),
	_autoMode(false), _state(Pause)
{
}

void PhSonySlaveController::processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *dataIn)
{
	unsigned char dataOut[16];
	PHDBG(20) << _deviceNumber << stringFromCommand(cmd1, cmd2, dataIn);
	switch (cmd1 >> 4) {
	case 0:
		switch (cmd2) {
		case 0x0c:
			PHDEBUG << _deviceNumber << "Local disable => ACK";
			sendAck();
			break;
		case 0x11:
			{
//			PHDEBUG << _ftdiDescription << "Device Type Request => F1C0";
				unsigned char deviceID1 = _settings->sonyDevice1();
				unsigned char deviceID2 = _settings->sonyDevice2();
				switch (_clock.timeCodeType()) {
				case PhTimeCodeType2398:
				case PhTimeCodeType24:
					deviceID1 += 2;
					break;
				case PhTimeCodeType25:
					deviceID1 += 1;
					break;
				case PhTimeCodeType2997:
					break;
				}
				sendCommand(0x12, 0x11, deviceID1, deviceID2);
				break;
			}
		case 0x1d:
			PHDEBUG << _deviceNumber << "Local enable => ACK";
			sendAck();
			break;
		default:
			PHDEBUG << _deviceNumber << " => Unknown subcommand " << stringFromCommand(cmd1, cmd2, dataIn) << " => NAK";
			sendNak(UndefinedCommand);
			break;
		}
		break;
	case 2:
		switch (cmd2) {
		case 0x00:
			PHDEBUG << _deviceNumber << "Stop => ACK";
			_state = Pause;
			_clock.setRate(0);
			sendAck();
			break;
		case 0x01:
			PHDEBUG << _deviceNumber << "Play => ACK";
			_state = Play;
			_clock.setRate(1);
			sendAck();
			break;
		case 0x10:
			PHDEBUG << _deviceNumber << "Fast forward => ACK";
			_state = FastForward;
			_clock.setRate(_settings->sonyFastRate());
			sendAck();
			break;
		case 0x20:
			PHDEBUG << _deviceNumber << "Rewing => ACK";
			_state = Rewind;
			_clock.setRate(-_settings->sonyFastRate());
			sendAck();
			break;
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x21:
		case 0x22:
		case 0x23:
			{
				PhRate rate = 0;
				switch (cmd1 & 0xf) {
				case 1:
					rate = computeRate(dataIn[0]);
					break;
				case 2:
					rate = computeRate(dataIn[0], dataIn[1]);
					break;
				}
				switch (cmd1) {
				case 0x11:
					_state = Jog;
					PHDEBUG << _deviceNumber << "Jog Forward : "<< rate<<"=> ACK";
					break;
				case 0x12:
					_state = Varispeed;
					PHDEBUG << _deviceNumber << "Var Forward : "<< rate<<"=> ACK";
					break;
				case 0x13:
					_state = Shuttle;
					PHDEBUG << _deviceNumber << "Shuttle Forward : " << rate << "=> ACK";
					break;
				case 0x21:
					rate = -rate;
					_state = Jog;
					PHDEBUG << _deviceNumber << "Jog rev : " << rate << "=> ACK";
					break;
				case 0x22:
					rate = -rate;
					_state = Varispeed;
					PHDEBUG << _deviceNumber << "Var rev : " << rate << "=> ACK";
					break;
				case 0x23:
					rate = -rate;
					_state = Shuttle;
					PHDEBUG << _deviceNumber << "Shuttle rev : " << rate << "=> ACK";
					break;
				}
				_clock.setRate(rate);
				sendAck();
				break;
			}
		case 0x31:
			{
				PhFrame frame = PhTimeCode::frameFromBcd(*(unsigned int *)dataIn, _clock.timeCodeType());
				_clock.setFrame(frame);
				PHDEBUG << _deviceNumber << "Cue at " << PhTimeCode::stringFromFrame(_clock.frame(), _clock.timeCodeType()) << "=> ACK";
				sendAck();
				break;
			}
		default:
			PHDEBUG << _deviceNumber << " => Unknown subcommand " << stringFromCommand(cmd1, cmd2, dataIn) << " => NAK";
			sendNak(UndefinedCommand);
			break;
		}
		break;
	case 4:
		switch (cmd2) {
		case 0x30:
			PHDEBUG << _deviceNumber << "Edit preset => ACK";
			sendAck();
			break;
		case 0x40:
			PHDEBUG << _deviceNumber << "Auto Mode Off => ACK";
			_autoMode = false;
			sendAck();
			break;
		case 0x41:
			_autoMode = true;
			PHDEBUG << _deviceNumber << "Auto Mode On => ACK";
			sendAck();
			break;  case 6:
		default:
			PHDEBUG << _deviceNumber << " => Unknown subcommand " << stringFromCommand(cmd1, cmd2, dataIn) << " => NAK";
			sendNak(UndefinedCommand);
			break;
		}
		break;
	case 6:
		switch (cmd2) {
		case 0x0c:
			{
				cmd1 = 0x74;
				PHDBG(21) << _deviceNumber << "Current Time Sense => " << _clock.timeCode();
				switch (dataIn[0]) {
				case 0x01:
					cmd2 = 0x04;
					break;
				case 0x02:
					cmd2 = 0x06;
					break;
				case 0x04:
					cmd2 = 0x00;
					break;
				case 0x08:
					cmd2 = 0x01;
					break;
				case 0x10:
					cmd2 = 0x05;
					break;
				case 0x20:
					cmd2 = 0x07;
					break;
				default:
					cmd2 = 0x04;
					break;
				}
				unsigned int bcd = PhTimeCode::bcdFromFrame(_clock.frame(), _clock.timeCodeType());
				sendCommandWithData(0x74, cmd2, (unsigned char *)&bcd);
				break;
			}
		case 0x20:
			{
				unsigned char status[16];
#warning /// @todo handle status sens properly
				PHDBG(22) << _deviceNumber << "Status Sense (%x) => Status Data" << QString::number(dataIn[0], 16);
				memset(status, 0, 16);
				switch (_state) {
				case Pause:
					status[1] = 0x80;
					status[2] = 0x03;
					break;
				case Play:
					status[1] = 0x81;
					status[2] = 0xc0;
					break;
				case FastForward:
					status[1] = 0x84;
					break;
				case Rewind:
					status[1] = 0x88;
					status[2] = 0x04;
					break;
				case Jog:
					status[1] = 0x80;
					if (_clock.rate() < 0)
						status[2] = 0x14;
					else
						status[2] = 0x10;
					break;
				case Varispeed:
					status[1] = 0x80;
					if (_clock.rate() < 0)
						status[2] = 0xcc;
					else
						status[2] = 0xc8;
					break;
				case Shuttle:
					status[1] = 0x80;
					if (_clock.rate() < 0)
						status[2] = 0x20;
					else
						status[2] = 0xa4;
					break;
				}
				if (_autoMode)
					status[3] = 0x80;
				unsigned char start = dataIn[0] >> 4;
				unsigned char count = dataIn[0] & 0xf;
				for (int i = 0; i < count; i++)
					dataOut[i] = status[i+start];
				sendCommandWithData(0x70+count, 0x20, status);
				break;
			}
		case 0x2e:
			{
				unsigned data1 = computeData1FromRate(_clock.rate());
//			PHDEBUG << "Speed sense";
				sendCommand(0x71, 0x2e, data1);
				break;
			}
		case 0x30:
			{
#warning /// @todo handle edit preset sense properly
//			PHDEBUG << _ftdiDescription << "Edit Preset Sense => Edit Preset Status";
				unsigned char count = dataIn[0];
				for (int i = 0; i < count; i++)
					dataOut[i] = 0;
				sendCommandWithData(0x70 + count, 0x30, dataOut);
				break;
			}
		default:
			PHDEBUG << _deviceNumber << " => Unknown subcommand " << stringFromCommand(cmd1, cmd2, dataIn) << " => NAK";
			sendNak(UndefinedCommand);
			break;
		}
		break;
	default:
		PHDEBUG << _deviceNumber << " => Unknown command " << stringFromCommand(cmd1, cmd2, dataIn) << " => NAK";
		sendNak(UndefinedCommand);
		break;
	}

//	PHDEBUG << _ftdiDescription << stringFromCommand(cmd1, cmd2, dataIn) << " over";
}

void PhSonySlaveController::onVideoSync()
{
	_clock.tick(PhTimeCode::getFps(_clock.timeCodeType()));
	PHDBG(20) << _clock.timeCode();
}

void PhSonySlaveController::sendAck()
{
//	PHDEBUG << _ftdiDescription;
	sendCommand(0x10, 0x01);
}

void PhSonySlaveController::sendNak(PhSonyController::PhSonyError error)
{
	PHDEBUG << _deviceNumber << error;
	sendCommand(0x11, 0x12, error);
}

void PhSonySlaveController::checkSumError()
{
	PhSonyController::checkSumError();
	sendNak(ChecksumError);
}

void PhSonySlaveController::timeOut()
{
	PhSonyController::timeOut();
	sendNak(TimeOut);
}
