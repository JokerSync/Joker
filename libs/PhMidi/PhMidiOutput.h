/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIOUTPUT_H
#define PHMIDIOUTPUT_H

#include "PhTools/PhData.h"

#include "PhMidiObject.h"

/**
 * @brief PhMidiOutput sends midi message
 *
 * This class can open an existing midi port and send
 * various messages through it.
 */
class PhMidiOutput : public PhMidiObject
{
	Q_OBJECT
public:
	PhMidiOutput();
	~PhMidiOutput();

	/**
	 * @brief List the available midi output port
	 * @return A list of string.
	 */
	static QStringList outputList();

	/**
	 * @brief Open an existing midi port.
	 * @param portName The midi port name
	 * @return True if success, false otherwise.
	 */
	bool open(QString portName);

	/**
	 * @brief close Close the midi port if opened
	 */
	void close();

	/**
	 * @brief Send a MTC quarter frame message
	 * @param data The data1 byte containing the MTC data.
	 */
	void sendQFTC(unsigned char data);

	/**
	 * @brief Send a full midi timecode message
	 * @param hh The hour digit
	 * @param mm The minute digit
	 * @param ss The second digit
	 * @param ff The frame digit
	 * @param tcType The timecode type
	 */
	void sendFullTC(unsigned char hh, unsigned char mm, unsigned char ss, unsigned char ff, PhTimeCodeType tcType);

	/**
	 * @brief Send a MMC goto message
	 * @param hh The hour digit
	 * @param mm The minute digit
	 * @param ss The second digit
	 * @param ff The frame digit
	 * @param tcType The timecode type
	 */
	void sendMMCGoto(unsigned char hh, unsigned char mm, unsigned char ss, unsigned char ff, PhTimeCodeType tcType);

public slots:
	/**
	 * @brief Send a MMC play message
	 */
	void sendMMCPlay();

	/**
	 * @brief Send a MMC stop message
	 */
	void sendMMCStop();

private:
	RtMidiOut *_midiOut;
};

#endif // PHMIDIOUTPUT_H
