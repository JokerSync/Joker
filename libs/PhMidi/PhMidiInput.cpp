/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhMidiInput.h"

PhMidiInput::PhMidiInput() :
	_midiIn(NULL),
	_hh(0),
	_mm(0),
	_ss(0),
	_ff(0),
	_tcType(PhTimeCodeType25)
{
}

PhMidiInput::~PhMidiInput()
{
	close();
}

bool PhMidiInput::open(QString portName)
{
	try {
		_midiIn = new RtMidiIn();
		PHDEBUG << "Opening" << portName;
		_midiIn->openVirtualPort(portName.toStdString());
		_midiIn->ignoreTypes( false, false, false );
		_midiIn->setCallback(&PhMidiInput::callback, this);
		_midiIn->setErrorCallback(&PhMidiInput::errorCallback, this);
		return true;
	}
	catch(RtMidiError &error) {
		error.printMessage();
		close();
		return false;
	}
}

void PhMidiInput::close()
{
	if(_midiIn) {
		if(_midiIn->isPortOpen()) {
			_midiIn->closePort();
		}
		delete _midiIn;
		_midiIn = NULL;
	}
}

void PhMidiInput::onMessage(std::vector<unsigned char> *message)
{
	if ( message->size() > 0 ) {
		unsigned char status = message->at(0);
		switch (status) {
		// A SysEx message
		case 0xf0:
			if(message->size() < 4)
				PHDEBUG << "Bad SysEx message size:" << message->size();
			else {
				unsigned char manufactorId = message->at(1);
				unsigned char channel = message->at(2);
				unsigned char type = message->at(3);
				switch (type) {
				// Timecode message type
				case 0x01:
					if(message->size() != 10)
						PHDEBUG << "Bad TC message size:" << message->size();
					else switch(message->at(4)) {
						case 0x01:
							_tcType = computeTimeCodeType(message->at(5) >> 5);
							_hh = message->at(5) & 0x1F;
							_mm = message->at(6);
							_ss = message->at(7);
							_ff = message->at(8);
							if(message->at(9) != 0xF7)
								PHDEBUG << "End of SysEx expected:" << QString::number(0xF7);
							PHDEBUG << "Full TC:" << _hh << _mm << _ss << _ff;
							onTC(_hh, _mm, _ss, _ff, _tcType);
							break;
						default:
							PHDEBUG << "Unknown TC type:" << message->at(4);
							break;
						}
					break;
				default:
					PHDEBUG << "Unknown SysEx type:" << QString::number(type, 16);
					break;
				}
			}
			break;
		// A quarter frame midi timecode message
		case 0xf1:
			if(message->size() != 2)
				PHDEBUG << "Bad QF MTC message size:" << message->size();
			else {
				unsigned char data1 = message->at(1);
				switch (data1 >> 4) {
				case 0:
					_ff = (_ff & 0xf0) | (data1 & 0x0f);
					break;
				case 1:
					_ff = (_ff & 0x0f) | ((data1 & 0x0f) << 4);
					break;
				case 2:
					_ss = (_ss & 0xf0) | (data1 & 0x0f);
					break;
				case 3:
					_ss = (_ss & 0x0f) | ((data1 & 0x0f) << 4);
					// Because of the way MTC is structured,
					// the minutes place won't be updated on the frame
					// where it changes over.
					// Dumb? Yes. But this fixes it.
					// From https://github.com/Figure53/TimecodeDisplay/blob/master/MIDIReceiver.m#L197
					//				if((_ss == 0) && (_ff == 0))
					//					_mm++;
					emit onTC(_hh, _mm, _ss, _ff, _tcType);
					break;
				case 4:
					_mm = (_mm & 0xf0) | (data1 & 0x0f);
					break;
				case 5:
					_mm = (_mm & 0x0f) | ((data1 & 0x0f) << 4);
					break;
				case 6:
					_hh = (_hh & 0xf0) | (data1 & 0x0f);
					break;
				case 7:
					_hh = (_hh & 0x0f) | ((data1 & 0x01) << 4);
					_tcType = computeTimeCodeType((data1 & 0x06) >> 1);

					emit onTC(_hh, _mm, _ss, _ff, _tcType);
					break;
				}

				emit onQuarterFrame();
//				PHDEBUG << "QF MTC:" << _hh << _mm << _ss << _ff;
			}
			break;
		default:
			{
				QString errorMessage = "Unknown midi message:";
				foreach(unsigned char data, *message)
				errorMessage += " " + QString::number(data, 16);
				PHDEBUG << errorMessage;
				break;
			}
		}
	}
}

void PhMidiInput::onError(RtMidiError::Type type, QString errorText)
{
	PHDEBUG << "Error:" << type << errorText;
}

void PhMidiInput::callback(double, std::vector<unsigned char> *message, void *userData)
{
	PhMidiInput *midiInput = (PhMidiInput*)userData;
	if(midiInput)
		midiInput->onMessage(message);
}

void PhMidiInput::errorCallback(RtMidiError::Type type, const std::string &errorText, void *userData)
{
	PhMidiInput *midiInput = (PhMidiInput*)userData;
	if(midiInput)
		midiInput->onError(type, QString::fromStdString(errorText));
}
