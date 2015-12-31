/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhMidiObject.h"

#include "PhTools/PhDebug.h"

PhMidiObject::PhMidiObject() :
	QObject(NULL)
{
}

bool PhMidiObject::canUseVirtualPorts()
{
	bool result = false;
	try {
		QScopedPointer<RtMidiOut> midiOut(new RtMidiOut());

		RtMidi::Api midiApi = midiOut->getCurrentApi();

		if ((midiApi == RtMidi::MACOSX_CORE)
		    || (midiApi == RtMidi::LINUX_ALSA)
		    || (midiApi == RtMidi::UNIX_JACK)) {
			result = true;
		}
	}
	catch(RtMidiError &error) {
		PHDEBUG << "Midi error:" << QString::fromStdString(error.getMessage());
	}

	return result;
}

PhTimeCodeType PhMidiObject::computeTimeCodeType(unsigned char data)
{
	switch (data) {
	case 0:
		return PhTimeCodeType24;
	case 1:
		return PhTimeCodeType25;
	case 2:
		return PhTimeCodeType2997;
	case 3:
		return PhTimeCodeType30;
	default:
		PHDEBUG << "Unknown tc type (assuming 25):" << data;
		return PhTimeCodeType25;
	}
}

unsigned char PhMidiObject::computeHH(unsigned char hh, PhTimeCodeType tcType)
{
	switch (tcType) {
	case PhTimeCodeType2398:
	case PhTimeCodeType24:
		return hh;
	case PhTimeCodeType25:
		return hh | (1 << 5);
	case PhTimeCodeType2997:
		return hh | (2 << 5);
	case PhTimeCodeType30:
		return hh | (3 << 5);
	}
	return 0;
}

unsigned char PhMidiObject::computeH(unsigned char hh, PhTimeCodeType tcType)
{
	unsigned char data = (hh & 0x10) >> 4;
	switch (tcType) {
	case PhTimeCodeType2398:
	case PhTimeCodeType24:
		data |= (0 << 1); // useless
		break;
	case PhTimeCodeType25:
		data |= (1 << 1);
		break;
	case PhTimeCodeType2997:
		data |= (2 << 1);
		break;
	case PhTimeCodeType30:
		data |= (3 << 1);
		break;
	}
	return data;
}
