/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef VIDEOSTRIPVIEW_H
#define VIDEOSTRIPVIEW_H

#include <QSettings>
#include <QTime>

#include "PhGraphic/PhGraphicView.h"
#include "PhVideo/PhVideoEngine.h"
#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhSync/PhSonyController.h"

#include "JokerSettings.h"

/**
 * @brief The Joker main view
 *
 * This PhGraphicView display the following graphic elements:
 * - the PhVideoEngine
 * - the PhGraphicStrip
 * - the current timecode
 * - the next text timecode (if a people list is currently selected, the
 * next text timecode display the one from the list)
 * - An error message if no video sync is received in sony mode
 * The view layout is configured via the settings.
 */
class VideoStripView : public PhGraphicView
{
	Q_OBJECT
public:
	/**
	 * @brief VideoStripView constructor
	 * @param parent The parent object
	 */
	explicit VideoStripView(QWidget *parent = 0);

	/**
	 * @brief Get the view's video engine
	 *
	 * @return The PhVideoEngine used by the view.
	 */
	PhVideoEngine * videoEngine() {
		return &_videoEngine;
	}

	/**
	 * @brief Get the view's strip
	 *
	 * @return The PhGraphicStrip used by the view.
	 */
	PhGraphicStrip * strip() {
		return &_strip;
	}

	/**
	 * @brief Attach the given settings to the view
	 *
	 * @param settings The QSettings
	 */
	void setSettings(JokerSettings *settings);

	/**
	 * @brief Set the PhSonyController
	 *
	 * @param sony The controller
	 */
	void setSony(PhSonyController * sony);

	/*!
	 * \brief Set if the video should be displayed with a forced aspect ratio
	 * \param force
	 */
	void setForceRatio169(bool force) {
		_forceRatio169 = force;
	}

protected:
	/**
	 * @brief Initialize the VideoStripView elements.
	 *
	 * @return True if success false otherwise
	 */
	bool init();
	/**
	 * @brief Paint the VideoStripView elements.
	 *
	 * Automatic call, process media information and displays it on the screen.
	 */
	void paint();

	/**
	 * @brief Get the mouse event of the view
	 *
	 * This event filter check for mouse move event in order to:
	 * - Display vertical resize cursor around the edge between the video and the strip
	 * - Resize the strip if the user drag the edge
	 * @param sender The event sender
	 * @param event The event
	 * @return Always false
	 */
	bool eventFilter(QObject *sender, QEvent *event);
private slots:
	void onVideoSync();

private:
	JokerSettings *_settings;
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

	bool _forceRatio169;
};

#endif // VIDEOSTRIPVIEW_H
