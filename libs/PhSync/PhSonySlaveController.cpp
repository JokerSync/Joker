#include "PhSonySlaveController.h"

#include <QDebug>

PhSonySlaveController::PhSonySlaveController(QObject *parent) : PhSonyController("A", parent)
{
}

void PhSonySlaveController::processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data)
{
	qDebug() << stringFromCommand(cmd1, cmd2, data);
	sendNak(UndefinedCommand);
}
