#ifndef PHPEOPLE_H
#define PHPEOPLE_H

#include "PhTools/PhColor.h"
#include "PhTools/PhString.h"


/**
 * Represents a character or a group of character from the video content. It can also be use to write notes or comments on the strip. For example : NDA (note de l'adaptateur).
 */
class PhPeople {

public:
    /**
     * @brief Default constructor
     */
    PhPeople();
    /**
     * @brief Get the name
     * @return a PhString
     */
    PhString getName();
    /**
     * @brief Set the name
     * @param name a PhString
     */
    void setName(PhString name);
    /**
     * @brief Get the color
     * @return a PhColor
     */
    PhColor getColor();
    /**
     * @brief Set the color
     * @param color a PhColor
     */
    void setColor(PhColor color);

private:
    /**
     * Name of the people
     */
    PhString name;
    /**
     * Color of the people's text on the strip.
     */
    PhColor color;

};

#endif // PHPEOPLE_H
