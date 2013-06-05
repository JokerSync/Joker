#ifndef PHCOLOR_H
#define PHCOLOR_H
#include <QColor>


#include "SDL/SDL.h"

#include "PhString.h"

class PhColor : public QColor
{
public:
    PhColor(PhString color);
    PhColor(int r, int g, int b, int a);
    PhColor();
    SDL_Color toSDL();

};

#endif // PHCOLOR_H
