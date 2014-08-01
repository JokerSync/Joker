/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhMidiInput.h"

PhMidiInput::PhMidiInput() :
	_midiIn(NULL)
{
}

bool PhMidiInput::open(QString portName)
{
	try {
		_midiIn = new RtMidiIn();
		PHDEBUG << "Opening" << portName;
		_midiIn->openVirtualPort(portName.toStdString());
		_midiIn->ignoreTypes( false, false, false );
		_midiIn->setCallback(&PhMidiInput::callback);
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
	QString log = QString("Incoming midi message (%1) :").arg(message->size());
	foreach (unsigned char c, *message)
		log += QString::number(c, 16, 2) + " ";
	PHDEBUG << log;
}

void PhMidiInput::callback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
	PHDEBUG;
	PhMidiInput *midiInput = (PhMidiInput*)userData;
	midiInput->onMessage(message);
}
