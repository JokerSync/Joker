/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIINPUT_H
#define PHMIDIINPUT_H

#include "PhTools/PhTimeCode.h"

#include "PhMidiObject.h"

class PhMidiInput : public PhMidiObject
{
	Q_OBJECT
public:
	PhMidiInput();
	~PhMidiInput();

	bool open(QString portName);
	void close();

signals:
	void onQuarterFrame();
	void onTC(int hh, int mm, int ss, int ff, PhTimeCodeType tcType);

private slots:
	void onMessage(std::vector<unsigned char> *message);
	void onError(RtMidiError::Type type, QString errorText);

private:
	static void callback(double, std::vector< unsigned char > *message, void *userData );
	static void errorCallback(RtMidiError::Type type, const std::string &errorText, void *userData);

	RtMidiIn *_midiIn;
	int _hh, _mm, _ss, _ff;
	PhTimeCodeType _tcType;
};

#endif // PHMIDIINPUT_H
