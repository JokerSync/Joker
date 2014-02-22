/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOSETTINGS_H
#define PHVIDEOSETTINGS_H

#include "PhGraphic/PhGraphicSettings.h"

class PhVideoSettings
{
public:
	virtual bool videoDeinterlace() = 0;
	virtual int screenDelay() = 0;
};

#endif // PHVIDEOSETTINGS_H
