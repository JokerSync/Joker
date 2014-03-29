#ifndef PHGRAPHICSETTINGS_H
#define PHGRAPHICSETTINGS_H

#include <QApplication>
#include <QString>

/**
 * @brief The settings for PhGraphic
 */
class PhGraphicSettings
{
public:
	/**
	 * @brief Display an overlay with some debug information
	 * @return True if displayed
	 */
	virtual bool displayInfo() = 0;

	/**
	 * @brief The font used for displaying the information
	 * @return A font file path
	 */
	virtual QString infoFontFile()
	{
		return QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/ARIAL.TTF";
	}
};

#endif // PHGRAPHICSETTINGS
