/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIINPUT_H
#define PHMIDIINPUT_H

#include "PhMidiObject.h"

/**
 * @brief A midi input interface
 *
 * This class provide all the methods for creating a port and
 * listen to new midi message. Signals are triggered when the following
 * messages are received:
 * - MTC
 * - MMC (@todo)
 */
class PhMidiInput : public PhMidiObject
{
	Q_OBJECT
public:
	/**
	 * @brief PhMidiInput constructor
	 */
	PhMidiInput();

	/**
	 * @brief PhMidiInput destructor
	 *
	 * Close the midi port if opened.
	 */
	~PhMidiInput();

	/**
	 * @brief Create a virtual midi port and open it.
	 * @param portName The midi port name
	 * @return True if success, false otherwise.
	 */
	bool open(QString portName);

	/**
	 * @brief close Close the midi port if opened
	 */
	void close();

signals:
	/**
	 * @brief Signal emitted upon new quarter frame message
	 */
	void onQuarterFrame();
	/**
	 * @brief Signal emitted upon full TC reception
	 *
	 * This signal can be emitted after:
	 * - several quarter frame midi timecode
	 * - a full MTC timecode
	 * - a MMC goto
	 *
	 * @param hh The hour digits.
	 * @param mm The minute digits.
	 * @param ss The second digits.
	 * @param ff The frame digits.
	 * @param tcType The timecode type.
	 */
	void onTC(int hh, int mm, int ss, int ff, PhTimeCodeType tcType);

	/**
	 * @brief Signal emitted upon MMC stop message reception
	 */
	void onStop();
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
