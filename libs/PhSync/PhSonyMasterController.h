#ifndef PHSONYMASTERCONTROLLER_H
#define PHSONYMASTERCONTROLLER_H

#include "PhSonyController.h"

/**
 * @brief Master controller for sony communication
 * Send command to a connected sony slave device
 * and update a clock component and the status accordingly.
 * The controller connect automatically with the first
 * usb serial connected port referenced as "B"
 */
class PhSonyMasterController : public PhSonyController
{
	Q_OBJECT
public:
	/** PhSonyMasterController constructor */
	PhSonyMasterController(QObject *parent);

public slots:
	/** Request the slave device id */
	void deviceTypeRequest();

	/** Send a play command to the connected device. */
	void play();

	/** Send a stop command to the connected device. */
	void stop();

	/** Send a fast forward command to the connected device. */
	void fastForward();

	/** Send a rewind command to the connected device. */
	void rewind();

	/**
	 * Send a jog command to the connected device.
	 * @param rate The jog rate
	 */
	void jog(PhRate rate);

	/**
	 * Send a varispeed command to the connected device.
	 * @param rate The varispeed rate
	 */
	void varispeed(PhRate rate);

	/**
	 * Send a shuttle command to the connected device.
	 * @param rate The shuttle rate
	 */
	void shuttle(PhRate rate);

	/** Send a time sense command to the connected device.
	 * For more detail see http://www.belle-nuit.com/archives/9pin.html#currentTimeSense
	 */
	void timeSense();

	/** Send a status sense command to the connected device.
	 * For more detail see http://www.belle-nuit.com/archives/9pin.html#statusSense
	 */
	void statusSense();

signals:
	void deviceIdAnswer(unsigned char id1, unsigned char id2);

protected:
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data);
};

#endif // PHSONYMASTERCONTROLLER_H





