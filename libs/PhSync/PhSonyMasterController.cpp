#include "PhSonyMasterController.h"

PhSonyMasterController::PhSonyMasterController(QObject *parent)
	: PhSonyController("B", parent )
{

}

void PhSonySlaveController::processComdmand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data)
{
	// TODO
}
