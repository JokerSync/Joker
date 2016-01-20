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
	 * @brief List the available midi input port
	 * @return A list of string.
	 */
	static QStringList inputList();

	/**
	 * @brief Create a virtual midi port and open it.
	 * @param inputPortName The midi port name
	 * @return True if success, false otherwise.
	 */
	bool open(QString inputPortName);

	/**
	 * @brief close Close the midi port if opened
	 */
	void close();

	/**
	 * @brief When a 24fps midi message is received, treat it as 23.98
	 * @param force True if force, false otherwise
	 */
	void force24as2398(bool force);

signals:
	/**
	 * @brief Signal emitted upon new quarter frame message
	 * @param data The quarter frame data
	 */
	void quarterFrame(unsigned char data);
	/**
	 * @brief Signal emitted upon full TC reception
	 *
	 * @param hh The hour digits.
	 * @param mm The minute digits.
	 * @param ss The second digits.
	 * @param ff The frame digits.
	 * @param tcType The timecode type.
	 *
	 * This signal can be emitted after:
	 * - several quarter frame midi timecode
	 * - a full MTC timecode
	 * - a MMC goto
	 */
	void timeCodeReceived(int hh, int mm, int ss, int ff, PhTimeCodeType tcType);

	/**
	 * @brief Signal emitted upon MMC play message reception
	 */
	void play();

	/**
	 * @brief Signal emitted upon MMC stop message reception
	 */
	void stop();

protected:
	/**
	 * @brief Called when a MTC quarter frame message is received
	 * @param data The quarter frame data.
	 *
	 * The class send a quarterFrame() signal but children can
	 * implement their custom reaction.
	 */
	virtual void onQuarterFrame(unsigned char data);

	/**
	 * @brief Called when a midi message updating the current timecode is received
	 *
	 * @param hh The hour digits.
	 * @param mm The minute digits.
	 * @param ss The second digits.
	 * @param ff The frame digits.
	 * @param tcType The timecode type.
	 *
	 * This signal can be emitted after:
	 * - several quarter frame midi timecode
	 * - a full MTC timecode
	 * - a MMC goto
	 */
	virtual void onTimeCode(int hh, int mm, int ss, int ff, PhTimeCodeType tcType);

	/**
	 * @brief Called when a MMC play message is received
	 */
	virtual void onPlay();

	/**
	 * @brief Called when a MMC stop message is received
	 */
	virtual void onStop();

	/**
	 * @brief Compute the time code type from a MTC data byte.
	 * @param data The MTC data byte
	 * @return A timecode type value
	 */
	PhTimeCodeType computeTimeCodeType(unsigned char data);

protected:
	/** @brief Remember the hour digits */
	unsigned int _hh;
	/** @brief Remember the minute digits */
	unsigned int _mm;
	/** @brief Remember the second digits */
	unsigned int _ss;
	/** @brief Remember the frame digits */
	unsigned int _ff;
	/** @brief Remember the time code type */
	PhTimeCodeType _mtcType;

private slots:
	void onMessage(std::vector<unsigned char> *message);
	void onError(RtMidiError::Type type, QString errorText);

private:
	static void callback(double, std::vector< unsigned char > *message, void *userData );
	static void errorCallback(RtMidiError::Type type, const std::string &errorText, void *userData);

	RtMidiIn *_midiIn;
	bool _force24as2398;
};

#endif // PHMIDIINPUT_H
