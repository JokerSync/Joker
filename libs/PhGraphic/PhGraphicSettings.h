#ifndef PHGRAPHICSETTINGS_H
#define PHGRAPHICSETTINGS_H

#include <QString>

#warning TODO check if remove
/**
 * @brief The setting for PhGraphicView child
 */
class PhGraphicSettings
{
public:
	/**
	 * @brief Screen delay compensation
	 * @return A value in millisecond
	 */
	virtual int screenDelay() = 0;
};

#endif // PHGRAPHICSETTINGS_H
