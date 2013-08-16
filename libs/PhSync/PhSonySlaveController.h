#ifndef PHSONYSLAVECONTROLLER_H
#define PHSONYSLAVECONTROLLER_H

#include "PhSonyController.h"

class PhSonySlaveController : public PhSonyController
{
public:
	PhSonySlaveController(QObject *parent);

protected:
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data);
};

#endif // PHSONYSLAVECONTROLLER_H
