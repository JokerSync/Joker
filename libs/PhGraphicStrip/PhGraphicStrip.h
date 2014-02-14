/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICSTRIP_H
#define PHGRAPHICSTRIP_H

#include <QObject>
#include <QSettings>

#include "PhStrip/PhStripDoc.h"

#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicImage.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicLoop.h"
#include "PhStrip/PhStripOff.h"

#include "PhTools/PhClock.h"

/**
 * @brief The PhGraphicStrip class
 *
 * This class draw a segment of strip band. The length of the strip band portion
 * is proportionnal to its width in pixel.
 *
 * The strip is divided in several horizontal track.
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
 * The  display also the loop (vertical dash with cross) and the cut changes (vertical dash).
 *
 * The  display a portion of strip band according to the current time.
 *
 * The portion of strip band scroll smoothly according to the current rate.
 *
 * The font used by the text is customisable.
 */
class PhGraphicStrip : public QObject
{
	Q_OBJECT
public:
	/**
	 * PhGraphicStrip constructor
	 * @param parent Parent object
	 */
	explicit PhGraphicStrip(QObject * parent = 0);

	/**
	 * Get the PhStripDoc attached to the .
	 * @return A PhStripDoc instance.
	 */
	PhStripDoc *doc();

	/**
	 * Get the PhClock attached to the .
	 * @return A PhClock instance.
	 */
	PhClock * clock();

	/**
	 * \brief Set the settings
	 * \param settings desired settings
	 */
	void setSettings(QSettings * settings);

	/**
	 * Set the font used to render text on the strip.
	 * @param fontFile Font file path
	 * @return true if the operation succeeds, false otherwise.
	 */
	bool setFontFile(QString fontFile);
	/**
	 * \brief Initializisation of the PhGraphicStrip
	 *
	 * Call clearData() then :
	 * - Load the strip background
	 * - Set the synchronization bar
	 * - Load the font file
	 *
	 * \return True if succeed, false otherwise
	 */
	bool init();

	/**
	 * \brief draw the strip
	 *
	 * It compute all the necessary data for the current frame, skipped
	 * if height = 0
	 *
	 * \param x upper left corner coordinates
	 * \param y upper left corner coordinates
	 * \param width width of the strip (usually the same as the parent window)
	 * \param height height of the strip
	 */
	void draw(int x, int y, int width, int height);

	/**
	 * \brief Set speed of the strip
	 *
	 * Allow the user to vary the speed of the strip (faster or slower)
	 * The default value is 12ppf
	 *
	 * \param value desired number of pixel per frame
	 */
	void setPixelPerFrame(long value);

	/**
	 * \brief Get the font of the strip objects
	 * \return the font
	 */
	PhFont * getTextFont();
	/**
	 * \brief Get the "head up display" Font
	 *
	 * The HUD font is used for all text printed on the screen which is not
	 * a direct part of the strip :
	 * - The current timecode
	 * - The next element timecode
	 * - The prediction
	 * - The title of the document...
	 * \return
	 */
	PhFont * getHUDFont();

	/**
	 * \brief setSelectedPeople
	 * Selected people will be displayed on the upper left corner, the others ones
	 * will be shaded. The next time code - if displayed - will be the next element
	 * of the people from the list.
	 * \param list
	 */
	void setSelectedPeople(QList<PhPeople *> * list){
		_selectedPeoples = list;
	}

private slots:
	/**
	 * @brief Clear all the graphic strip object related to the PhStripDoc.
	 */
	void clearData();

private:

	/**
	 * @brief _doc
	 * Reference to the current PhStripDoc
	 */
	PhStripDoc _doc;

	PhClock _clock;

	/**
	 * @brief The font used to draw text
	 */
	PhFont _textFont;

	/**
	 * @brief The font used to draw the rest of the HUD
	 */
	PhFont _hudFont;

	/**
	 * Background Image used for the strip band
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
	QTime _testTimer;

	int _trackNumber;
	QSettings * _settings;
	long pixelPerFrame;
	QList<PhPeople*> *_selectedPeoples;

};

#endif // PHGRAPHICSTRIP_H
