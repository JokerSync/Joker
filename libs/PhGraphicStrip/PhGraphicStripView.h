/**
* @brief This file contains the declaration of the PhGraphicStripView class.
*
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICSTRIPVIEW_H
#define PHGRAPHICSTRIPVIEW_H

#include <QSettings>

#include "PhStrip/PhStripDoc.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicImage.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicLoop.h"
#include "PhStrip/PhStripOff.h"

#include "PhTools/PhClock.h"

/**
 * @brief The PhGraphicStripView class
 *
 * This class draw a segment of strip band. The length of the strip band portion
 * is proportionnal to its width in pixel.
 *
 * The view is divided in several horizontal track.
 *
 * Each track can contains text element.
 *
 * Each text element can be preceeded by a people name using the following rule:
 *
 * If the text element is affected to a people and there is no text affected to the same
 * people in the two second preceeding the start of the text, the people name is displayed.
 *
 * In all other case, no people name is displayed.
 *
 * The view display also the loop (vertical dash with cross) and the cut changes (vertical dash).
 *
 * The view display a portion of strip band according to the current time.
 *
 * The portion of strip band scroll smoothly according to the current rate.
 *
 * The font used by the text is customisable.
 */
class PhGraphicStripView : public PhGraphicView
{

    Q_OBJECT

public:
	/**
	 * PhGraphicStripView constructor
	 * @param parent Parent object
	 */
	explicit PhGraphicStripView(QWidget *parent = 0);

	/**
	 * Get the PhStripDoc attached to the view.
	 * @return A PhStripDoc instance.
	 */
	PhStripDoc *doc();

	/**
	 * Get the PhClock attached to the view.
	 * @return A PhClock instance.
	 */
	PhClock * clock();

	void setSettings(QSettings * settings);

	/**
	 * Set the font used to render text on the strip.
	 * @param fontfile Font file path
	 * @return true if the operation succeeds, false otherwise.
	 */
	bool setFontFile(QString fontFile);


protected:

	bool init();

	void paint();



private slots:
	/**
	 * @brief Clear all the graphic strip object related to the PhStripDoc.
	 */
	void clearData();

private :

 	/**
	 * @brief _doc
	 * Reference to the current PhStripDoc
	 */
	PhStripDoc _doc;

	PhClock _clock;

	/**
	 * @brief The PhFont used to draw text
	 */
	PhFont _font;

	/**
	 *Background Image used for the strip band
     */
	PhGraphicImage _stripBackgroundImage;

	PhGraphicSolidRect _stripSyncBar;

	QMap<PhPeople*, PhGraphicText*> _graphicPeoples;

	QMap<PhStripText*, PhGraphicText*> _graphicTexts;

	QMap<PhStripCut*, PhGraphicSolidRect*> _graphicCuts;

	QMap<PhStripLoop*, PhGraphicLoop*> _graphicLoops;

	QMap<PhStripOff*, PhGraphicSolidRect*> _graphicOffs;

	/**
	 * @brief _test
	 * QTime for testing performance
	 */
	QTime _test;

	int _trackNumber;

	QSettings * _settings;
};

#endif // PhGraphicStripView_H
