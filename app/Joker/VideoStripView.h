/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef VIDEOSTRIPVIEW_H
#define VIDEOSTRIPVIEW_H

#include <QSettings>
#include <QTime>

#include "PhGraphic/PhGraphicView.h"
#include "PhVideo/PhVideoEngine.h"
#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhSync/PhSonyController.h"

/*!
 * \brief The VideoStripView class
 * Process the video and strip displays
 */


class VideoStripView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit VideoStripView(QWidget *parent = 0);


	/** \addtogroup Getters
	 *  @{
	 */
	/*!
	 * \brief videoEngine
	 * \return
	 * TODO
	 */
	PhVideoEngine * videoEngine() { return &_videoEngine;}
	/*!
	 * \brief strip
	 * \return
	 * TODO
	 */
	PhGraphicStrip * strip() { return &_strip;}
	/*!
	 * \brief getSelectedPeoples
	 * \return
	 * Return a QList of PhPeople* containing the selected PhPeople
	 */
	QList<PhPeople*>* getSelectedPeoples();
	/** @}*/


	/** \addtogroup Setters
	 *  @{
	 */

	/*!
	 * \brief setSettings
	 * \param settings
	 * Attach the given settings
	 */
	void setSettings(QSettings * settings);

	/*!
	 * \brief Set the PhSonyController
	 * \param sony The controller
	 */
	void setSony(PhSonyController * sony);

	/** @}*/


protected:
	/*!
	 * \brief init
	 * \return
	 * initialize the VideoStripView
	 */
	bool init();
	/*!
	 * \brief paint
	 * Automatic call, process media information and displays it on the screen.
	 */
	void paint();

private slots:
	void onVideoSync();
	void onDocChanged();

private:
	PhVideoEngine _videoEngine;
	PhGraphicStrip _strip;
	PhSonyController *_sony;
	QTime _lastVideoSyncElapsed;

	PhGraphicText _titleText;
	PhGraphicSolidRect _titleBackgroundRect;
	PhGraphicText _tcText;
	PhGraphicText _nextTCText;
	PhGraphicText _noVideoSyncError;
	PhGraphicText _currentPeopleName;

	QList<PhPeople*> _selectedPeoples;
};

#endif // VIDEOSTRIPVIEW_H
