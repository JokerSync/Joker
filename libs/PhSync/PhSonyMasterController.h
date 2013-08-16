#ifndef PHSONYMASTERCONTROLLER_H
#define PHSONYMASTERCONTROLLER_H

#include "PhSonyController.h"

class PhSonyMasterController : public PhSonyController
{
public:
	PhSonyMasterController(QObject *parent);

protected:
	void processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data);
};

#endif // PHSONYMASTERCONTROLLER_H
