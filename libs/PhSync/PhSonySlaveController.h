#ifndef PHSONYSLAVECONTROLLER_H
#define PHSONYSLAVECONTROLLER_H

#include "PhSonyController.h"

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

	void onCTS();
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
