/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSONYCONTROLLER_H
#define PHSONYCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QThread>

#include "PhSync/PhClock.h"

#include "PhSonySettings.h"

/**
 * @brief Sony abstract controller for  sony 9 pin communication through the serial port.
 *
 * It provide a generic implementation for handlind sony master
 * and slave communication.
 * It contains an internal clock which behave differently if it is a sony
 * master or slave.
 *
 * Sony 9 pin specification : http://www.belle-nuit.com/archives/9pin.html
 */
class PhSonyController : public QThread
{
	Q_OBJECT
public:

	/**
	 * @brief Various error code sent with the NAK command.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#nak
	 */
	enum PhSonyError {
		UndefinedCommand,
		UnusedError1,
		ChecksumError,
		UnusedError3,
		ParityError,
		OverrunError,
		FramingError,
		TimeOut
	};

	/**
	 * @brief PhSonyController constructor
	 *
	 * @param settings The application settings
	 * @param comSuffix Serial port name suffix
	 */
	explicit PhSonyController(PhSonySettings *settings, QString comSuffix);

	/**
	 * @brief PhSonyController destructor
	 *
	 * Closing the port if open.
	 */
	~PhSonyController();

	/**
	 * @brief Open the communication port.
	 */
	bool open(bool inThread = true);

	/**
	 * @brief Close the communication port.
	 */
	void close();

	/**
	 * @brief The timecode type used by the protocol
	 * @return A timecode type value.
	 */
	virtual PhTimeCodeType timeCodeType() = 0;

	/**
	 * @brief Get the sony controller internal clock.
	 * @return A clock reference.
	 */
	PhClock *clock() {
		return &_clock;
	}

	/**
	 * @brief Compute the rate from the jog, varispeed and shuttle sony protocole
	 * order data.
	 *
	 * For more detail see http://www.belle-nuit.com/archives/9pin.html#jogFwd
	 * @param data1 A one byte coded version of the rate.
	 * @return The float value corresponding rate.
	 */
	static PhRate computeRate(unsigned char data1);

	/**
	 * Compute the rate from the jog, varispeed and shuttle sony protocole
	 * order data.
	 * For more detail see http://www.belle-nuit.com/archives/9pin.html#jogFwd
	 * @param data1 The first byte of the two bytes coded version of the rate.
	 * @param data2 The second byte of the two bytes coded version of the rate.
	 * @return The float value corresponding rate.
	 */
	static PhRate computeRate(unsigned char data1, unsigned char data2);

	/**
	 * Compute the jog, varispeed and shuttle sony protocole
	 * order data from a rate
	 * For more detail see http://www.belle-nuit.com/archives/9pin.html#jogFwd
	 * @param rate The float value rate.
	 * @return A one byte coded version of the rate.
	 */
	static unsigned char computeData1FromRate(PhRate rate);

signals:
	/**
	 * @brief This signal is triggered when a video sync event occurs on the serial port.
	 */
	void videoSync();

public slots:
	/**
	 * @brief This slot trigger a check of the video sync check.
	 *
	 * If this slot is not trigger
	 * regulary, no video sync event occurs.
	 */
	void checkVideoSync();

	/**
	 * @brief This slot handles the video sync signal.
	 *
	 * Its implementation differs between the master and the slave.
	 */
	virtual void onVideoSync() = 0;

protected:
	/**
	 * @brief The thread starting point
	 *
	 * This method is called when the thread is created.
	 * It constantly read the data on the serial port and pass
	 * it to the child via the processCommand() virtual method.
	 */
	void run();

	/**
	 * @brief Process a single sony command.
	 *
	 * The clock and controller state are updated if needed.
	 * This method shall be implemented differently by the slave and the master.
	 *
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @param dataIn Command data.
	 */
	virtual void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char* dataIn) = 0;

	/**
	 * @brief Extract the data size from the first command descriptor.
	 * @param cmd1 First command descriptor.
	 * @return Data size in byte.
	 */
	unsigned char getDataSize(unsigned char cmd1);

	/**
	 * Send a sony protocol command.
	 *
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @param data Data for the command.
	 */
	void sendCommandWithData(unsigned char cmd1, unsigned char cmd2, const unsigned char *data);

	/**
	 * @brief Send a sony protocol command with an argument list of unsigned char for the data.
	 *
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 */
	void sendCommand(unsigned char cmd1, unsigned char cmd2, ...);

	/**
	 * @brief This method is called whenever a timeout happens when reading the data.
	 *
	 * It allows distinct implementation for slave and master.
	 */
	virtual void timeOut();

	/**
	 * @brief This method is called whenever a check sum error happens when reading the data.
	 *
	 * It allows distinct implementation for slave and master.
	 */
	virtual void checkSumError();

	/**
	 * @brief Convert a sony command and data to a readable string.
	 *
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @param data Command data.
	 * @return The name of the command.
	 */
	QString stringFromCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data = 0);

	/** @brief The internal clock of the sony controller. */
	PhClock _clock;

	/** @brief The application settings */
	PhSonySettings* _settings;

	/** @brief Serial port name suffix (A for slave and B for master). */
	QString _comSuffix;

private:
	/** @brief Serial port connected to the controller. */
	QSerialPort _serial;

	/** @brief Buffer used for serial data reception. */
	unsigned char _dataIn[256];

	/** @brief Amount of serial data read for the current command/data */
	int _dataRead;

	/** @brief Buffer used for serial data emission. */
	unsigned char _dataOut[256];

	/** @brief Last value of the serial CTS state. */
	bool _lastCTS;

	/** @brief Indicate if the thread is currently running */
	bool _threadRunning;

private slots:
	/** @brief Slot triggered when data are available on the serial port */
	void onData();

	/** @brief Slot triggered when a serial error occurs */
	void handleError(QSerialPort::SerialPortError error);
};

#endif // PHSONYCONTROLLER_H

