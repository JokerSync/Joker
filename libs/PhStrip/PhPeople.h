#ifndef PHPEOPLE_H
#define PHPEOPLE_H

#include "PhTools/PhColor.h"
#include "PhTools/PhString.h"


/**
 * Represents a character or a group of character from the video content. It can also be use to write notes or comments on the strip. For example : NDA (note de l'adaptateur).
 */
class PhPeople {

private:
    /**
     * Name of the people
     */
    PhString name;
    /**
     * Color of the people's text on the strip.
     */
    PhColor color;

public:
    PhString getName();
    void setName(PhString name);
    PhColor getColor();
    void setColor(PhColor color);
};

#endif // PHPEOPLE_H
