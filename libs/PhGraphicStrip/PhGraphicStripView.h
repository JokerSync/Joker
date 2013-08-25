/**
* @brief This file contains the declaration of the PhGraphicStripView class.
*
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICSTRIPVIEW_H
#define PHGRAPHICSTRIPVIEW_H

#include "PhStrip/PhStripDoc.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicImage.h"
#include "PhGraphic/PhGraphicSolidRect.h"

//#include "PhTools/PhTimeCode.h"

//#include "MainController.h"
//#include "PhGraphic/PhGraphicController.h"
#include "PhGraphicStrip/PhGraphicStripController.h"

//#include "PhCommonUI/PhLeap.h"





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
     * @brief PhGraphicStripView
     * @param parent
     * @param file
     * constructor
     */
    explicit PhGraphicStripView(QWidget *parent = 0);

protected:

	bool init();

	void paint();

	/**
	 * @brief PhGraphicStripView::clearData
	 *Clear _texts and _imgs
	 */
	void clearData();

	/**
	 * @brief PhGraphicStripView::changeScroll
	 * Toggle between Play and Pause
	 */
	void changeScroll();
	/**
	 * @brief PhGraphicStripView::setScroll
	 * @param shouldScroll
	 * Set the scroll information
	 */
	void setScroll(bool shouldScroll);

	/**
	 * @brief PhGraphicStripView::getFonts
	 * @return QList<PhFont *>
	 */
	QList<PhFont *> getFonts();
	/**
	 * Set the current font with the font in param and re-initialize the OpenGL content
	 * @param fontfile Font file path
	 */
	void setCurrentFont(QString fontfile);
	/**
	 * @brief PhGraphicStripView::getCurrentFont
	 * @return PhFont
	 */
	PhFont * getCurrentFont();

	/**
	 * @brief PhGraphicStripView::setXmove
	 * @param n
	 * Scroll the strip of n pixels, if n>0 the strip will scroll to the left
	 * and vice-versa.
	 */
	void setXmove(int n);

	/**
	 * @brief PhGraphicStripView::getDoc
	 * @return PhStripDoc
	 */
	PhStripDoc *getDoc();

	void toggleNaturalScrolling();
	//void setController(MainController * controller);
	void setController(PhGraphicStripController * controller);
	void connectSlots();
	void setNaturalScroll(bool naturalScroll);


	void openFile(char * file);

public slots:
	void stopScroll();


private :
    //MainController * _controller;
 PhGraphicStripController * _controller;
	/**
	 * @brief _doc
	 * Reference to the current PhStripDoc
	 */
	PhStripDoc *_doc;

	/**
	 * @brief _XMove
	 * float which define the scroll
	 */
	float _xmove;

	float _xMoveStrip;

	/**
	 * @brief _shouldmove
	 * allow us to know if the strip should scroll or not.
	 */
	bool _shouldmove;

	/**
	 * @brief _fonts
	 * QList of loaded PhFonts.
	 */
	QList<PhFont *> _fonts;

	/**
	 * @brief _currentFont
	 * The current PhFont used to draw text
	 */
	PhFont *_currentFont;

	/**
	 * @brief _texts
	 * QList of texts loaded
	 */
	QList<PhGraphicText *> _texts;

    /**
	 *Background Image used for the strip band
     */
	PhGraphicImage * _stripBackgroundImage;

	QList<PhGraphicRect *> _cuts;

	/**
	 * @brief _test
	 * QTime for testing performance
	 */
	QTime *_test;



	bool _naturalScroll;

};

#endif // PhGraphicStripView_H
