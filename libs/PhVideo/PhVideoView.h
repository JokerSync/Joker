/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHVIDEOVIEW_H
#define PHVIDEOVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhVideoEngine.h"

/*!
 * \brief The PhVideoView class
 *
 * Provide the viewer of the video
 */
class PhVideoView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit PhVideoView(QWidget *parent = 0);
	/*!
	 * \brief Set the PhVideoEngine
	 * \param videoEngine The given videoEngine
	 */
	void setEngine(PhVideoEngine * videoEngine);

protected:
	/*!
	 * \brief Initialize the video view
	 * \return true if succeed, false otherwise.
	 */
	bool init();
	/*!
	 * \brief Paint the video on the screen
	 */
	void paint();

private:
	PhVideoEngine *_videoEngine;
};

#endif // PHVIDEOVIEW_H
