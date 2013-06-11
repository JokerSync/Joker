/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef STRIPWINDOW_H
#define STRIPWINDOW_H

#include "glu.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "PhStrip/PhStripDoc.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicImage.h"


class StripWindow : public PhGraphicView
{Q_OBJECT

public:
    /**
     * @brief StripWindow
     * @param parent
     * @param file
     * constructor
     */
    explicit StripWindow(QWidget *parent = 0, PhString file = "");

    /**
     * @brief initializeGL
     * initialize the objects to draw
     */
    void initializeGL();

    /**
     * @brief paintGL
     * refresh the framebuffer
     */
    void paintGL();
    /**
     * @brief StripWindow::openFile
     * @param filename
     * Set the position to 0, load a new PhStripDoc, call initializeGL if another file was loaded
     */
    void openFile(PhString filename);
    /**
     * @brief StripWindow::clearData
     *Clear _texts and _imgs
     */
    void clearData();

    /**
     * @brief StripWindow::changeScroll
     * Toggle between Play and Pause
     */
    void changeScroll();
    /**
     * @brief StripWindow::setScroll
     * @param shouldScroll
     * Set the scroll information
     */
    void setScroll(bool shouldScroll);

    /**
     * @brief StripWindow::getFonts
     * @return QList<PhFont *>
     */
    QList<PhFont *> getFonts();
    /**
     * @brief StripWindow::setCurrentFont
     * @param font
     * Set the current font with the font in param and re-initialize the OpenGL content
     */
    void setCurrentFont(PhFont * font);
    /**
     * @brief StripWindow::getCurrentFont
     * @return PhFont
     */
    PhFont * getCurrentFont();

    /**
     * @brief StripWindow::setXmove
     * @param n
     * Scroll the strip of n pixels, if n>0 the strip will scroll to the left
     * and vice-versa.
     */
    void setXmove(int n);

    /**
     * @brief StripWindow::getDoc
     * @return PhStripDoc
     */
    PhStripDoc *getDoc();



private :

    /**
     * @brief _doc
     * Reference to the current PhStripDoc
     */
    PhStripDoc *_doc;

    /**
     * @brief _xmove
     * float which define the scroll
     */
    float _xmove;

    /**
     * @brief _firstload
     * allow us to know if it's the first load or not.
     */
    bool _firstload;
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
    QList<PhGraphicImage *> _imgs;

    /**
     * @brief _test
     * QTime for testing performance
     */
    QTime *_test;

};

#endif // STRIPWINDOW_H
