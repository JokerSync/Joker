#include "PhSonySlaveController.h"

#include "PhTools/PhDebug.h"

PhSonySlaveController::PhSonySlaveController(QObject *parent) : PhSonyController("A", parent),
	_autoMode(false), _state(Pause)
{
}

void PhSonySlaveController::processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *dataIn)
{
	unsigned char dataOut[16];
	PHDEBUG << _comSuffix << stringFromCommand(cmd1, cmd2, dataIn);
	switch (cmd1 >> 4)
	{
	case 0:
		switch (cmd2)
		{
		case 0x0c:
			PHDEBUG << _comSuffix << "Local disable => ACK";
			sendAck();
			break;
		case 0x11:
		{
			PHDEBUG << _comSuffix << "Device Type Request => F1C0";
			// TODO : Device ID as a parameter
			unsigned char deviceID1 = 0xf0;
			unsigned char deviceID2 = 0xc0;
			switch (_clock.getTCType())
			{
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
			PHDEBUG << _comSuffix << "Local enable => ACK";
			sendAck();
			break;
		default:
			PHDEBUG << _comSuffix << " => Unknown subcommand => NAK";
			sendNak(UndefinedCommand);
			break;
		}
		break;
	case 2:
		switch (cmd2)
		{
		case 0x00:
			PHDEBUG << _comSuffix << "Stop => ACK";
			_state = Pause;
			_clock.setRate(0);
			sendAck();
			break;
		case 0x01:
			PHDEBUG << _comSuffix << "Play => ACK";
			_state = Play;
			_clock.setRate(1);
			sendAck();
			break;
//					case 0x10:
//						PHDEBUG << _comSuffix << "Fast forward => ACK");
//						state = FastForward;
//						_clock.rate() = [[NSUserDefaults standardUserDefaults] doubleForKey:@"DWSonyRewindFastForwardSpeed"];
//						[port sendAck];
//						break;
//					case 0x20:
//						PHDEBUG << _comSuffix << "Rewing => ACK");
//						state = Rewind;
//						_clock.rate() = -[[NSUserDefaults standardUserDefaults] doubleForKey:@"DWSonyRewindFastForwardSpeed"];
//						[port sendAck];
//						break;
//					case 0x11:
//					case 0x12:
//					case 0x13:
//					case 0x21:
//					case 0x22:
//					case 0x23:
//					{
//						double rate = 0;
//						switch (cmd1 & 0xf) {
//							case 1:
//								rate = [self computeRateWithData1:dataIn[0]];
//								break;
//							case 2:
//								rate = [self computeRateWithData1:dataIn[0] andData2:dataIn[1]];
//								break;
//						}
//						switch (cmd1) {
//							case 0x11:
//								state = Jog;
//								PHDEBUG << _comSuffix << "Jog Forward : %.2f => ACK", rate);
//								break;
//							case 0x12:
//								state = Var;
//								PHDEBUG << _comSuffix << "Var Forward : %.2f => ACK", rate);
//								break;
//							case 0x13:
//								state = Shuttle;
//								PHDEBUG << _comSuffix << "Shuttle Forward : %.2f => ACK", rate);
//								break;
//							case 0x21:
//								rate = -rate;
//								state = Jog;
//								PHDEBUG << _comSuffix << "Jog rev : %.2f => ACK", rate);
//								break;
//							case 0x22:
//								rate = -rate;
//								state = Var;
//								PHDEBUG << _comSuffix << "Var rev : %.2f => ACK", rate);
//								break;
//							case 0x23:
//								rate = -rate;
//								state = Shuttle;
//								PHDEBUG << _comSuffix << "Shuttle rev : %.2f => ACK", rate);
//								break;
//						}
//						_clock.rate() = rate;
//						[port sendAck];
//						break;
//					}
//					case 0x31:
//					{
//						unsigned char hh = [DWBCDTool uintFromBcd:dataIn[3]];
//						unsigned char mm = [DWBCDTool uintFromBcd:dataIn[2]];
//						unsigned char ss = [DWBCDTool uintFromBcd:dataIn[1]];
//						unsigned char ff = [DWBCDTool uintFromBcd:dataIn[0]];
//						self.clock.frame = [DWTimeCode frameFromHh:hh Mm:mm Ss:ss Ff:ff andType:self.clock.type];
//						PHDEBUG << _comSuffix << "Cue at %@ => ACK", self.clock.tcString);
//						[port sendAck];
//						break;
//					}
//					default:
//						PHDEBUG << _comSuffix << "Unknown subcommand : %x %x => NAK", cmd1, cmd2);
//						[port sendNak:0x00];
//						break;
//				}
//				break;
//			case 4:
//				switch (cmd2) {
//					case 0x30:
//						PHDEBUG << _comSuffix << "Edit preset : %@", [DWString stringWithBuffer:dataIn andLength:[port getDataCount:cmd1]]);
//						[port sendAck];
//						break;
//					case 0x40:
//						autoMode = NO;
//						PHDEBUG << _comSuffix << "Auto Mode Off => ACK");
//						[port sendAck];
//						break;
//					case 0x41:
//						autoMode = YES;
//						PHDEBUG << _comSuffix << "Auto Mode On => ACK");
//						[port sendAck];
//						break;
		default:
			PHDEBUG << _comSuffix << "Unknown subcommand => NAK";
			sendNak(UndefinedCommand);
			break;
		}
		break;
	case 6:
		switch (cmd2)
		{
		case 0x0c:
		{
			cmd1 = 0x74;
			PHDEBUG << _comSuffix << "Current Time Sense => " << PhTimeCode::stringFromFrame(_clock.frame(), _clock.getTCType());
			switch (dataIn[0])
			{
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
			unsigned int hhmmssff[4];
			PhTimeCodeType tcType = _clock.getTCType();
			PhTimeCode::ComputeHhMmSsFf(hhmmssff, _clock.frame(), tcType);
			unsigned char hh = PhTimeCode::bcdFromFrame(hhmmssff[0], tcType);
			unsigned char mm = PhTimeCode::bcdFromFrame(hhmmssff[1], tcType);
			unsigned char ss = PhTimeCode::bcdFromFrame(hhmmssff[2], tcType);
			unsigned char ff = PhTimeCode::bcdFromFrame(hhmmssff[3], tcType);
			sendCommand(0x74, cmd2, ff, ss, mm, hh);
			break;
		}
		case 0x20:
		{
			unsigned char status[16];
			// TODO : handle status sens properly
			PHDEBUG << _comSuffix << "Status Sense (%x) => Status Data" << QString::number(dataIn[0], 16);
			memset(status, 0, 8);
			switch (_state)
			{
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
			// TODO check status with usb422v test
			unsigned char start = dataIn[0] >> 4;
			unsigned char count = dataIn[0] & 0xf;
			for (int i=0; i<count; i++)
				dataOut[i] = status[i+start];
			sendCommand(0x70+count, 0x20, status);
			break;
		}
		case 0x2e:
		{
			unsigned data1 = computeData1FromRate(_clock.rate());
			PHDEBUG << "Speed sense";
			sendCommand(0x71, 0x2e, data1);
			break;
		}
//								case 0x30:
//								{
//									// TODO : handle properly
//									PHDEBUG << _comSuffix << "Edit Preset Sense => Edit Preset Status");
//									unsigned char count = dataIn[0];
//									for (int i=0; i<count; i++) {
//										dataOut[i] = 0;
//									}
//									[port sendCommand:0x70 + count cmd2:0x30 data:dataOut];
//									break;
//								}
		default:
				PHDEBUG << _comSuffix << "Unknown subcommand : => NAK";
				sendNak(UndefinedCommand);
				break;
			}
			break;
		default:
			PHDEBUG << _comSuffix << " => Unknown command : " << QString::number(cmd1, 16) << " " << QString::number(cmd2, 16) << " => NAK";
		sendNak(UndefinedCommand);
		break;
	}

	PHDEBUG << _comSuffix << stringFromCommand(cmd1, cmd2, dataIn) << " over";
}
