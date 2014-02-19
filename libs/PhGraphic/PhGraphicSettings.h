#ifndef PHGRAPHICSETTINGS_H
#define PHGRAPHICSETTINGS_H

#include <QString>

class PhGraphicSettings
{
public:
	virtual int screenDelay() = 0;
	virtual int screenRefreshInterval() = 0;
};

#endif // PHGRAPHICSETTINGS_H
