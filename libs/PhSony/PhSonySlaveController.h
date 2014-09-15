/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSONYSLAVECONTROLLER_H
#define PHSONYSLAVECONTROLLER_H

#include "PhSonyController.h"

/**
 * @brief Slave controller for sony 9 pin communication through the serial port.
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
 *
 * The device ID and the speed are loaded from the settings. If no settings are available,
 * a default value is used. The default values are:
 * - speed = 3
 * - device ID1 = 0xf0
 * - device ID2 = 0xc0
 *
 */
class PhSonySlaveController : public PhSonyController
{
public:
	/**
	 * @brief PhSonySlaveController constructor.
	 * @param settings The application settings
	 */
	PhSonySlaveController(PhSonySettings *settings);

	/**
	 * @brief Various state in which the controller can be.
	 *
	 * This state is used to answer to the master status command.
	 */
	enum PhSonyState {
		Pause,
		Play,
		FastForward,
		Rewind,
		Jog,
		Varispeed,
		Shuttle,
	};

	PhTimeCodeType timeCodeType();

public slots:
	/**
	 * @brief This slot update the clock accordingly.
	 */
	void onVideoSync();

protected:
	/**
	 * @brief Process a single command from the sony master and respond to it.
	 *
	 * The clock and controller state are updated if needed.
	 *
	 * @param cmd1 First command descriptor.
	 * @param cmd2 Second command descriptor.
	 * @param dataIn Command data.
	 */
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *dataIn);

private:
	/**
	 * @brief Send a command acknolegment.
	 */
	void sendAck();

	/**
	 * @brief Send an error to the command emiter.
	 * @param error Error type.
	 */
	void sendNak(PhSonyError error);

	void checkSumError();

	void timeOut();
private:
	bool _autoMode;
	PhSonyState _state;

};

#endif // PHSONYSLAVECONTROLLER_H
