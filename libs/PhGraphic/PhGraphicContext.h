/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICCONTEXT_H
#define PHGRAPHICCONTEXT_H

#include <QDebug>
#include <QImage>

#include "glu.h"

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"

class PhGraphicView;

class PhGraphicContext
{
public:
    PhGraphicContext(PhGraphicView *view);
    void saveToPNG();

private:
    void init();
    void dispose();

    PhGraphicView *_graphVeiw;

};

#endif // PHGRAPHICCONTEXT_H
