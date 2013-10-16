#ifndef PHGRAPHICSTRIP_H
#define PHGRAPHICSTRIP_H

#include <QObject>
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
	 * Set the current font with the font in param and re-initialize the OpenGL content
	 * @param fontfile Font file path
	 * @return true if the operation succeeds, false otherwise
	 */
	bool setFont(QString fontName);

	bool init();

	void draw(int x, int y, int width, int height);

protected:
	/**
	 * @brief PhGraphicStrip::clearData
	 *Clear _texts and _imgs
	 */
	void clearData();


private slots:
	void update();

private :

 	/**
	 * @brief _doc
	 * Reference to the current PhStripDoc
	 */
	PhStripDoc _doc;

	PhClock _clock;

	/**
	 * @brief _currentFont
	 * The current PhFont used to draw text
	 */
	PhFont *_currentFont;

	/**
	 *Background Image used for the strip band
     */
	PhGraphicImage * _stripBackgroundImage;

	PhGraphicSolidRect * _stripSyncBar;

	QMap<PhPeople*, PhGraphicText*> _graphicPeoples;

	QMap<PhStripText*, PhGraphicText*> _graphicTexts;

	QMap<PhStripCut*, PhGraphicRect*> _graphicCuts;

	QMap<PhStripLoop*, PhGraphicLoop*> _graphicLoops;

	QMap<PhStripOff*, PhGraphicRect*> _graphicOffs;

	/**
	 * @brief _test
	 * QTime for testing performance
	 */
	QTime *_test;

	int _trackNumber;
};

#endif // PHGRAPHICSTRIP_H
