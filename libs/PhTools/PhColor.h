#ifndef PHCOLOR_H
#define PHCOLOR_H
#include <QColor>


#include "SDL/SDL.h"

#include "PhString.h"

class PhColor : public QColor
{
public:
    /**
     * @brief PhColor
     * @param color
     */
    PhColor(PhString color);
    /**
     * @brief PhColor
     * @param r
     * @param g
     * @param b
     * @param a
     */
    PhColor(int r, int g, int b, int a);
    /**
     * @brief PhColor
     */
    PhColor();
    /**
     * @brief toSDL
     * @return
     * gives the equivalent of a PhColor in SDL_Color system
     */
    SDL_Color toSDL();

};

#endif // PHCOLOR_H
