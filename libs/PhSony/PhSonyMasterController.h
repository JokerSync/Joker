/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSONYMASTERCONTROLLER_H
#define PHSONYMASTERCONTROLLER_H

#include "PhSonyController.h"

/**
 * @brief Sony master controller for sony 9 pin communication through the serial port.
 *
 * Send command to a connected sony slave device
 * and update a clock component and the status accordingly.
 * The controller connect automatically with the first
 * usb serial connected port referenced as "B".
 *
 * Upon a video sync event, the master controller perform three actions:
 * - status sense
 * - time sense
 * - speed sense
 */
class PhSonyMasterController : public PhSonyController
{
	Q_OBJECT
public:
	/**
	 * @brief PhSonyMasterController constructor
	 * @param settings The application settings
	 */
	PhSonyMasterController(PhSonySettings *settings);

	PhTimeCodeType timeCodeType();

public slots:
	/**
	 * @brief This slot performs a status, time and speed sense.
	 */
	void onVideoSync();

	/** @brief Request the slave device id. */
	void deviceTypeRequest();

	/**
	 * @brief Send a play command to the connected device.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#play
	 */
	void play();

	/**
	 * @brief Send a stop command to the connected device.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#stop
	 */
	void stop();

	/**
	 * @brief Send a cue command to the connected device.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#cueUpWithData
	 *
	 * @param time A time value
	 */
	void cue(PhTime time);

	/**
	 * @brief Send a fast forward command to the connected device.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#fastFwd
	 */
	void fastForward();

	/**
	 * @brief Send a rewind command to the connected device.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#rewind
	 */
	void rewind();

	/**
	 * @brief Send a jog command to the connected device.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#jogFwd
	 *
	 * @param rate The jog rate
	 */
	void jog(PhRate rate);

	/**
	 * @brief Send a varispeed command to the connected device.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#varFwd
	 *
	 * @param rate The varispeed rate
	 */
	void varispeed(PhRate rate);

	/**
	 * @brief Send a shuttle command to the connected device.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#shuttleFwd
	 *
	 * @param rate The shuttle rate
	 */
	void shuttle(PhRate rate);

	/** @brief Send a time sense command to the connected device.
	 *
	 * See http://www.belle-nuit.com/archives/9pin.html#currentTimeSense
	 */
	void timeSense();

	/** @brief Send a status sense command to the connected device.
	 *
	 * See http://www.belle-nuit.com/archives/9pin.html#statusSense
	 */
	void statusSense();

	/** @brief Send a speed sense command to the connected device.
	 *
	 * See http://www.belle-nuit.com/archives/9pin.html#cmdSpeedSense
	 */
	void speedSense();

signals:
	/**
	 * @brief Signal sent when a device id information is received by the sony master controller.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#deviceTypeRequest
	 *
	 * @param id1 Major device description byte.
	 * @param id2 Minor device description byte.
	 */
	void deviceIdData(unsigned char id1, unsigned char id2);

	/**
	 * @brief Signal sent when status data ared received by the sony master controller.
	 *
	 * See : http://www.belle-nuit.com/archives/9pin.html#statusData
	 *
	 * @param statusData Partial status data buffer.
	 * @param offset Offset of the partial status data buffer according to the full status info.
	 * @param length Length of the partial status data buffer.
	 */
	void statusData(unsigned char * statusData, int offset, int length);

protected:
	/**
	 * @brief Process a single command from the sony slave.
	 *
	 * The clock and controller state are updated if needed.
	 *
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @param dataIn Command data.
	 */
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *dataIn);
};

#endif // PHSONYMASTERCONTROLLER_H





