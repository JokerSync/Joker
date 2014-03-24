/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICSTRIPVIEW_H
#define PHGRAPHICSTRIPVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"

/**
 * @brief Provide a view of the PhGraphicStrip class
 */
class PhGraphicStripView : public PhGraphicView
{
	Q_OBJECT
public:
	/**
	 * @brief PhGraphicStripView constructor
	 * @param parent The parent object
	 */
	explicit PhGraphicStripView(QWidget *parent = 0);

	/**
	 * @brief Get the PhGraphicStrip attached to the view
	 * @return
	 */
	PhGraphicStrip * strip() {
		return &_strip;
	}

	void setSettings(PhGraphicStripSettings *settings);
protected:
	/**
	 * @brief Initializisation of the PhGraphicStrip
	 * @return
	 */
	bool init();
	/**
	 * @brief call the PhGraphicView's method paint()
	 */
	void paint();

private:
	PhGraphicStrip _strip;
	PhGraphicStripSettings *_settings;
};

#endif // PHGRAPHICSTRIPVIEW_H
