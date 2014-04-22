/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOVIEW_H
#define PHVIDEOVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhFont.h"

#include "PhVideoEngine.h"

/**
 * @brief Provide a view for a PhVideoEngine
 */
class PhVideoView : public PhGraphicView
{
	Q_OBJECT
public:
	/**
	 * @brief PhVideoView constructor
	 * @param parent The parent object
	 */
	explicit PhVideoView(QWidget *parent = 0);
	/**
	 * @brief Set the PhVideoEngine
	 * @param videoEngine The given videoEngine
	 */
	void setEngine(PhVideoEngine * videoEngine);

protected:
	/**
	 * @brief Paint the video on the screen
	 */
	void paint();

private:
	PhVideoEngine *_videoEngine;
	int _maxVideoRate;
};

#endif // PHVIDEOVIEW_H
