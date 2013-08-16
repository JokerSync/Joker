#include "PhSonySlaveController.h"

PhSonySlaveController::PhSonySlaveController(QObject *parent) : PhSonyController("A", parent)
{
}

void PhSonySlaveController::processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data)
{
	// TODO
}
