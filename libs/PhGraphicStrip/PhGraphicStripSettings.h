#ifndef PHGRAPHICSTRIPSETTINGS_H
#define PHGRAPHICSTRIPSETTINGS_H

#include <QString>
#include <QDir>

#include "PhGraphic/PhGraphicSettings.h"

/**
 * @brief The settings for PhGraphicStrip
 */
class PhGraphicStripSettings : public PhGraphicSettings
{
public:
	/**
	 * @brief Screen delay compensation
	 * @return A value in millisecond
	 */
	virtual int screenDelay() = 0;
	/**
	 * @brief The relative height of the strip in the view
	 * @return A float value
	 */
	virtual float stripHeight() = 0;
	/**
	 * @brief The amount of pixel to move a graphic object
	 * per screen refresh at nominal speed
	 * @return An integer value
	 */
	virtual int horizontalSpeed() = 0;
	/**
	 * @brief Set the strip text font file
	 * @param value A font file path
	 */
	virtual void setTextFontFile(QString value) = 0;
	/**
	 * @brief Get the strip text font file
	 * @return A font file path
	 */
	virtual QString textFontFile() = 0;
	/**
	 * @brief Get the strip text boldness
	 * @return A integer value from 0 to 5
	 */
	virtual int textBoldness() = 0;
	/**
	 * @brief Display the strip in test mode
	 *
	 * During the test mode, the whole strip is black except
	 * if a cut is at the current time then the whole strip is white
	 *
	 * @return True if test mode, false otherwise
	 */
	virtual bool stripTestMode() = 0;
	/**
	 * @brief Display the next people intervention
	 *
	 * The next people right after the rythmo range are scrolling
	 * vertically on the left from the top of the view
	 * to the top of the strip.
	 *
	 * @return True if displayed, false otherwise
	 */
	virtual bool displayNextText() = 0;
	/**
	 * @brief The amount of pixel to move a graphic object
	 * per screen refresh at nominal speed
	 * @return An integer value
	 */
	virtual int verticalSpeed() = 0;

	/**
	 * @brief Allow color invertion
	 * @return True if the color is inverted, false otherwise
	 */
	virtual bool invertColor() = 0;
	/**
	 * @brief Display the ruler on the strip
	 * @return True if the ruler is displayed, false otherwise
	 */
	virtual bool displayRuler() = 0;
	/**
	 * @brief The timestamp of the ruler
	 * @return
	 */
	virtual int rulerTimeIn() = 0;
	/**
	 * @brief The amount of time between each draw of the ruler
	 * @return A time value
	 */
	virtual int timeBetweenRuler() = 0;

	/**
	 * @brief Display some informations about the strip
	 * @return True if the informations are displayed
	 */
	virtual bool displayStripInfo() = 0;
};

#endif // PHGRAPHICSTRIPSETTINGS_H
