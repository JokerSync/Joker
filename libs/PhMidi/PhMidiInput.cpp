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
	if ( message->size() > 1 ) {
		if(message->at(0) == 0xf1) {
			int data1 = message->at(1);
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
				if((_ss == 0) && (_ff == 0))
					_mm++;
				onTC(_hh, _mm, _ss, _ff, _tcType);
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
				switch((data1 & 0x06) >> 1) {
				case 0:
					_tcType = PhTimeCodeType24;
					break;
				case 1:
					_tcType = PhTimeCodeType25;
					break;
				case 2:
					_tcType = PhTimeCodeType2997;
					break;
				case 3:
					_tcType = PhTimeCodeType30;
					break;
				}

				onTC(_hh, _mm, _ss, _ff, _tcType);
				break;
			}
			PHDEBUG << "QF MTC" << QString::number(data1, 16) << _hh << ":" << _mm << ":" << _ss << ":" << _ff;

			onQuarterFrame();
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
