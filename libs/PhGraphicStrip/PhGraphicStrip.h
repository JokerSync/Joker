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

	void setSettings(QSettings * settings);

	/**
	 * Set the font used to render text on the strip.
	 * @param fontfile Font file path
	 * @return true if the operation succeeds, false otherwise.
	 */
	bool setFontFile(QString fontFile);

	bool init();

	void draw(int x, int y, int width, int height);

	void setPixelPerFrame(long value);

	PhFont * getFont();


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
	QTime _testTimer;

	int _trackNumber;
	QSettings * _settings;
	long pixelPerFrame;
};

#endif // PHGRAPHICSTRIP_H
