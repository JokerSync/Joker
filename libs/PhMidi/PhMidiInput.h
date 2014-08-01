/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIINPUT_H
#define PHMIDIINPUT_H

#include "PhMidiObject.h"

class PhMidiInput : public PhMidiObject
{
	Q_OBJECT
public:
	PhMidiInput();
	bool open(QString portName);
	void close();

private slots:
	void onMessage(std::vector<unsigned char> *message);

private:
	static void callback(double deltatime, std::vector< unsigned char > *message, void *userData );

	RtMidiIn *_midiIn;
};

#endif // PHMIDIINPUT_H
