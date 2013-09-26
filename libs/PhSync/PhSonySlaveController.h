#ifndef PHSONYSLAVECONTROLLER_H
#define PHSONYSLAVECONTROLLER_H
#include <QSettings>

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
 */
class PhSonySlaveController : public PhSonyController
{
public:
	/**
	 * @brief PhSonySlaveController constructor.
	 * @param parent The object owner.
	 */
	PhSonySlaveController(PhTimeCodeType tcType, QSettings *settings, QObject *parent);

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
	QSettings* _settings;
	bool _autoMode;
	PhSonyState _state;
	PhRate _fastForwardRate;
	PhRate _rewindRate;
};

#endif // PHSONYSLAVECONTROLLER_H
