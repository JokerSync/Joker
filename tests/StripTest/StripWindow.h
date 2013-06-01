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
    explicit StripWindow(QWidget *parent = 0, char * file = "");
    void initializeGL();
    void paintGL();
    void openFile(PhString filename);
    int getStripWidth();
    void setStripWidth(int w);

    int getStripHeight();
    void setStripHeight(int w);

private :

    PhStripDoc *_doc;
    float xmove;
    float y;

    QList<PhFont *> _fonts;
    QList<PhGraphicText *> _texts;
    QList<PhGraphicImage *> _imgs;

};

#endif // STRIPWINDOW_H
