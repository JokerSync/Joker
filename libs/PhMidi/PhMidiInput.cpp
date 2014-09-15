/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhMidiInput.h"

PhMidiInput::PhMidiInput() :
	_hh(0),
	_mm(0),
	_ss(0),
	_ff(0),
	_mtcType(PhTimeCodeType25),
	_midiIn(NULL)
{
}

PhMidiInput::~PhMidiInput()
{
	close();
}

QStringList PhMidiInput::inputList()
{
	QStringList result;

	RtMidiIn *midiIn;
	try {
		midiIn = new RtMidiIn();
		for(unsigned int i = 0; i < midiIn->getPortCount(); i++)
			result.append(QString::fromStdString(midiIn->getPortName(i)));
	}
	catch(RtMidiError &error) {
		PHDEBUG << "Midi error:" << QString::fromStdString(error.getMessage());
	}
	delete midiIn;

	return result;
}

bool PhMidiInput::open(QString inputPortName)
{
	PHDEBUG << inputPortName;
	try {
		_midiIn = new RtMidiIn();
		int portIndex = -1;
		for(unsigned int i = 0; i < _midiIn->getPortCount(); i++) {
			QString portName = QString::fromStdString(_midiIn->getPortName(i));
			PHDEBUG << "-" << portName;
			if(inputPortName == portName) {
				portIndex = i;
				break;
			}
		}
		PHDEBUG << "Opening" << inputPortName;
		if(portIndex >= 0)
			_midiIn->openPort(portIndex);
		else
			_midiIn->openVirtualPort(inputPortName.toStdString());
		_midiIn->ignoreTypes( false, false, false );
		_midiIn->setCallback(&PhMidiInput::callback, this);
		_midiIn->setErrorCallback(&PhMidiInput::errorCallback, this);
		return true;
	}
	catch(RtMidiError &error) {
		PHDEBUG << "Midi error:" << QString::fromStdString(error.getMessage());
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

void PhMidiInput::onQuarterFrame(unsigned char data)
{
	emit quarterFrame(data);
}

void PhMidiInput::onTimeCode(int hh, int mm, int ss, int ff, PhTimeCodeType tcType)
{
	emit timeCodeReceived(hh, mm, ss, ff, tcType);
}

void PhMidiInput::onPlay()
{
	emit play();
}

void PhMidiInput::onStop()
{
	emit stop();
}

void PhMidiInput::onMessage(std::vector<unsigned char> *message)
{
	if ( message->size() > 0 ) {
		QString messageStr = "";
		foreach(unsigned char data, *message)
			messageStr += QString::number(data, 16) + " ";

		PHDBG(21) << messageStr;

		unsigned char status = message->at(0);
		switch (status) {
		// A SysEx message
		case 0xf0:
			if(message->size() < 4)
				PHDEBUG << "Bad SysEx message size:" << message->size() << "/" << messageStr;
			else {
				unsigned char manufactorId = message->at(1);
#warning /// @todo Handle midi channel
//				unsigned char channel = message->at(2);
				unsigned char type = message->at(3);
				if(manufactorId == 0x7F) {
					switch (type) {
					// Timecode message type
					case 0x01:
						if(message->size() != 10)
							PHDEBUG << "Bad TC message size:" << message->size();
						else switch(message->at(4)) {
							case 0x01:
								_mtcType = computeTimeCodeType(message->at(5) >> 5);
								_hh = message->at(5) & 0x1F;
								_mm = message->at(6);
								_ss = message->at(7);
								_ff = message->at(8);
								if(message->at(9) != 0xF7)
									PHDEBUG << "End of SysEx expected:" << QString::number(0xF7);
								PHDEBUG << "Full TC:" << _hh << _mm << _ss << _ff;
								onTimeCode(_hh, _mm, _ss, _ff, _mtcType);
								break;
							default:
								PHDEBUG << "Unknown TC type:" << message->at(4) << "/" << messageStr;
								break;
							}
						break;
					// Midi machine control message type
					case 0x06:
						switch(message->at(4)) {
						case 0x01:
							PHDEBUG << "MMC Stop" << messageStr;
							onStop();
							break;
						case 0x02:
							PHDEBUG << "MMC Play" << messageStr;
							onPlay();
							break;
						case 0x44:
							_mtcType = computeTimeCodeType(message->at(7) >> 5);
							_hh = message->at(7) & 0x1F;
							_mm = message->at(8);
							_ss = message->at(9);
							// It seems that the some information is sent to the frame byte too (not timecode type)...
							_ff = message->at(10) & 0x1F;
							PHDEBUG << "Go To" << _hh << _mm << _ss << _ff;
							onTimeCode(_hh, _mm, _ss, _ff, _mtcType);
							break;
						default:
							PHDEBUG << "Unknown MMC message:" << messageStr;
							break;
						}

						break;
					default:
						PHDEBUG << "Unknown SysEx type:" << QString::number(type, 16) << "/" << messageStr;
						break;
					}
				}
//				else
//					PHDEBUG << "Not a MMC message:" << messageStr;
			}
			break;
		// A quarter frame midi timecode message
		case 0xf1:
			if(message->size() != 2)
				PHDEBUG << "Bad QF MTC message size:" << message->size() << "/" << messageStr;
			else {
				unsigned char data = message->at(1);
				switch (data >> 4) {
				case 0:
					_ff = (_ff & 0xf0) | (data & 0x0f);
//					onTimeCode(_hh, _mm, _ss, _ff, _tcType);
					break;
				case 1:
					_ff = (_ff & 0x0f) | ((data & 0x0f) << 4);
					break;
				case 2:
					_ss = (_ss & 0xf0) | (data & 0x0f);
					break;
				case 3:
					_ss = (_ss & 0x0f) | ((data & 0x0f) << 4);
					// Because of the way MTC is structured,
					// the minutes place won't be updated on the frame
					// where it changes over.
					// Dumb? Yes. But this fixes it.
					// From https://github.com/Figure53/TimecodeDisplay/blob/master/MIDIReceiver.m#L197
					//				if((_ss == 0) && (_ff == 0))
					//					_mm++;
					//onTimeCode(_hh, _mm, _ss, _ff, _tcType);
					break;
				case 4:
					_mm = (_mm & 0xf0) | (data & 0x0f);
					break;
				case 5:
					_mm = (_mm & 0x0f) | ((data & 0x0f) << 4);
					break;
				case 6:
					_hh = (_hh & 0xf0) | (data & 0x0f);
					break;
				case 7:
					_hh = (_hh & 0x0f) | ((data & 0x01) << 4);
					_mtcType = computeTimeCodeType((data & 0x06) >> 1);
					break;
				}

				PHDBG(20) << QString("QF MTC (%1): %2:%3:%4:%5").arg(QString::number(data, 16)).arg(_hh).arg(_mm).arg(_ss).arg(_ff);
				onQuarterFrame(data);
			}
			break;
		default:
			PHDEBUG << "Unknown midi message:" << messageStr;
			break;
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
