#ifndef PHSONYSLAVECONTROLLER_H
#define PHSONYSLAVECONTROLLER_H

#include "PhSonyController.h"

/**
 * @brief Master controller for sony communication
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
class PhSonySlaveController : public PhSonyController
{
public:
	PhSonySlaveController(QObject *parent);

	enum PhSonyState {
		Pause,
		Play,
		FastForward,
		Rewind,
		Jog,
		Varispeed,
		Shuttle,
	};

protected:
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data);

	void onVideoSync();
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
