/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhMidiOutput.h"

PhMidiOutput::PhMidiOutput() :
	_midiOut(NULL)
{
}

bool PhMidiOutput::open(QString portName)
{
	try {
		_midiOut = new RtMidiOut();
		for(int i = 0; i < _midiOut->getPortCount(); i++) {
			PHDEBUG << "-" << QString::fromStdString(_midiOut->getPortName(i));
			if(QString::fromStdString(_midiOut->getPortName(i)) == portName) {
				PHDEBUG << "Opening" << QString::fromStdString(_midiOut->getPortName(i));
				_midiOut->openPort(i);
				PHDEBUG << _midiOut->isPortOpen();
				return true;
			}
		}
	}
	catch(RtMidiError &error) {
		error.printMessage();
	}
	close();
	return false;
}

void PhMidiOutput::close()
{
	if(_midiOut) {
		if(_midiOut->isPortOpen()) {
			_midiOut->closePort();
		}
		delete _midiOut;
		_midiOut = NULL;
	}
}

void PhMidiOutput::test()
{
	if(_midiOut) {
		std::vector<unsigned char> message = { 0xf1, 0x00}; // This message correspond to a quarter frame MTC settings lower frame to 0
		_midiOut->sendMessage(&message);
	}
}
