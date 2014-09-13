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

PhMidiOutput::~PhMidiOutput()
{
	close();
}

QStringList PhMidiOutput::outputList()
{
	QStringList result;
	RtMidiOut *midiOut;
	try {
		midiOut = new RtMidiOut();
		for(unsigned int i = 0; i < midiOut->getPortCount(); i++)
			result.append(QString::fromStdString(midiOut->getPortName(i)));
	}
	catch(RtMidiError &error) {
		PHDEBUG << "Midi error:" << QString::fromStdString(error.getMessage());
	}
	delete midiOut;

	return result;
}

bool PhMidiOutput::open(QString portName)
{
	PHDEBUG << "Opening" << portName;
	try {
		_midiOut = new RtMidiOut();
		for(unsigned int i = 0; i < _midiOut->getPortCount(); i++) {
			if(QString::fromStdString(_midiOut->getPortName(i)) == portName) {
				_midiOut->openPort(i);
				return true;
			}
		}
		PHDEBUG << "Unable to find" << portName;
	}
	catch(RtMidiError &error) {
		PHDEBUG << "Midi error:" << QString::fromStdString(error.getMessage());
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

void PhMidiOutput::sendQFTC(unsigned char data)
{
	if(_midiOut) {
		std::vector<unsigned char> message = { 0xf1, data};
		_midiOut->sendMessage(&message);
	}
}

void PhMidiOutput::sendFullTC(unsigned char hh, unsigned char mm, unsigned char ss, unsigned char ff, PhTimeCodeType tcType)
{
	if(_midiOut) {
		std::vector<unsigned char> message = { 0xf0, 0x7f, 0x7f, 0x01, 0x01, computeHH(hh, tcType), mm, ss, ff, 0xf7 };
		_midiOut->sendMessage(&message);
	}
}

void PhMidiOutput::sendMMCGoto(unsigned char hh, unsigned char mm, unsigned char ss, unsigned char ff, PhTimeCodeType tcType)
{
	if(_midiOut) {
		std::vector<unsigned char> message = { 0xf0, 0x7f, 0x7f, 0x06, 0x44, 0x06, 0x01, computeHH(hh, tcType), mm, ss, ff, 0xf7 };
		_midiOut->sendMessage(&message);
	}
}

void PhMidiOutput::sendMMCPlay()
{
	if(_midiOut) {
		std::vector<unsigned char> message = { 0xf0, 0x7f, 0x7f, 0x06, 0x02, 0xf7 };
		_midiOut->sendMessage(&message);
	}
}

void PhMidiOutput::sendMMCStop()
{
	if(_midiOut) {
		std::vector<unsigned char> message = { 0xf0, 0x7f, 0x7f, 0x06, 0x01, 0xf7 };
		_midiOut->sendMessage(&message);
	}
}

