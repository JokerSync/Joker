#ifndef PHSONYCONTROLLER_H
#define PHSONYCONTROLLER_H

#include <QObject>
#include <QSerialPort>

#include <PhTools/PhClock.h>

/**
 * PhSonyController is an abstract class handle sony 9 pin communication
 * through the serial port.
 * It provide a generic implementation for handlind sony master
 * and slave communication.
 * It contains an internal clock which behave differently if it is a sony
 * master or slave.
 *
 * Sony 9 pin specification : http://www.belle-nuit.com/archives/9pin.html
 */
class PhSonyController : public QObject
{
	Q_OBJECT
public:
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
	 * @param comSuffix Serial port name suffix
	 * @param parent Parent for the QObject (mandatory)
	 */
	explicit PhSonyController(QString comSuffix, QObject *parent);

	/** @brief PhSonyController destructor
	 *
	 * Closing the port if open.
	 */
	~PhSonyController();

	/**
	 * Open the communication port.
	 */
	bool open();

	/**
	 * Close the communication port.
	 */
	void close();

	PhClock *clock() { return &_clock; }

public slots:
	/**
	 * This slot trigger a check of the video sync check. If this slot is not trigger
	 * regulary, no video sync event occurs.
	 */
	void checkVideoSync();

protected:
	/**
	 * Compute the rate from the jog, varispeed and shuttle sony protocole
	 * order data.
	 * For more detail see http://www.belle-nuit.com/archives/9pin.html#jogFwd
	 * @param data1 A one byte coded version of the rate.
	 * @return The float value corresponding rate.
	 */
	PhRate computeRate(unsigned char data1);

	/**
	 * Compute the rate from the jog, varispeed and shuttle sony protocole
	 * order data.
	 * For more detail see http://www.belle-nuit.com/archives/9pin.html#jogFwd
	 * @param data1 The first byte of the two bytes coded version of the rate.
	 * @param data2 The second byte of the two bytes coded version of the rate.
	 * @return The float value corresponding rate.
	 */
	PhRate computeRate(unsigned char data1, unsigned char data2);

	/**
	 * Compute the jog, varispeed and shuttle sony protocole
	 * order data from a rate
	 * For more detail see http://www.belle-nuit.com/archives/9pin.html#jogFwd
	 * @param rate The float value rate.
	 * @return A one byte coded version of the rate.
	 */
	unsigned char computeData1FromRate(PhRate rate);

	/**
	 * Process a single command and respond to it, updating the clock if needed.
	 * This method shall be implemented differently by the slave and the master.
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @param dataIn Command data.
	 */
	virtual void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char* data) = 0;

	/**
	 * Extract the data size from the first command descriptor.
	 * @param cmd1 First command descriptor.
	 * @return Data size in byte.
	 */
	unsigned char getDataSize(unsigned char cmd1);

	/**
	 * Send a sony protocol command.
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @param data Data for the command.
	 */
	void sendCommandWithData(unsigned char cmd1, unsigned char cmd2, const unsigned char *data);

	/**
	 * Send a sony protocol command with an argument list of unsigned char for the data.
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 */
	void sendCommand(unsigned char cmd1, unsigned char cmd2, ...);

	/**
	 * This method is called whenever a timeout happens when reading the data.
	 * It allows distinct implementation for slave and master.
	 */
	virtual void timeOut();

	/**
	 * This method is called whenever a check sum error happens when reading the data.
	 * It allows distinct implementation for slave and master.
	 */
	virtual void checkSumError();

	/**
	 * This method is called whenever a video sync signal is triggered.
	 * It allows distinct implementation for slave and master.
	 */
	virtual void onVideoSync();

	/**
	 * @brief stringFromCommand
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @return The name of the command.
	 */
	QString stringFromCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data = 0);

	/** The internal clock of the sony controller */
	PhClock _clock;

	/** Serial port name suffix (A for slave and B for master) */
	QString _comSuffix;

private:
	void checkData();

	// Serial port connected to the controller
	QSerialPort _serial;
	bool sonyRunning;
	unsigned char _dataIn[256];
	int _dataRead;
	unsigned char _dataOut[256];
	bool _lastCTS;
private slots:
	void onData();
	void handleError(QSerialPort::SerialPortError error);
};

#endif // PHSONYCONTROLLER_H

