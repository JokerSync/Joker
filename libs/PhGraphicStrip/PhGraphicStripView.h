/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICSTRIPVIEW_H
#define PHGRAPHICSTRIPVIEW_H

#include "glu.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "PhStrip/PhStripDoc.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicImage.h"

#include "PhTools/PhTimeCode.h"

//#include "MainController.h"
#include "PhGraphic/PhGraphicController.h"
#include "PhGraphicStrip/PhGraphicStripController.h"

#include "PhCommonUI/PhLeap.h"




/**
 * @brief The PhGraphicStripView class
 *
 *
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

    /**
     * @brief paintGL
     * refresh the framebuffer
     */
    void paintGL();

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
    void setCurrentFont(PhString fontfile);
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

  //  void openFile(char * file);

public slots:
    void stopScroll();


private slots:


    /**
     * @brief initializeGL
     * initialize the objects to draw
     */
    void initializeGL();


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
     * @brief _imgs
     * QList of images loaded
     */
    QList<PhGraphicImage *> _strips;

    QList<PhGraphicTexturedRect *> _cuts;

    /**
     * @brief _test
     * QTime for testing performance
     */
    QTime *_test;



    bool _naturalScroll;

};

#endif // PhGraphicStripView_H
