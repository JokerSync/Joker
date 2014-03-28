#ifndef PHGRAPHICSETTINGS_H
#define PHGRAPHICSETTINGS_H

#include <QString>

/**
 * @brief The settings for PhGraphic
 */
class PhGraphicSettings
{
public:
	virtual bool displayInfo() {return false;}
	virtual QString infoFontFile() {return "";}
};

#endif // PHGRAPHICSETTINGS
