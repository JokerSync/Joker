/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIOUTPUT_H
#define PHMIDIOUTPUT_H

#include "PhMidiObject.h"

class PhMidiOutput : public PhMidiObject
{
	Q_OBJECT
public:
	PhMidiOutput();
	~PhMidiOutput();

	bool open(QString portName);
	void close();
	void sendMTC(unsigned char data);

private:
	RtMidiOut *_midiOut;
};

#endif // PHMIDIOUTPUT_H
