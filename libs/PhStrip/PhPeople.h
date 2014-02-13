/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHPEOPLE_H
#define PHPEOPLE_H

#include <QString>

/**
 * Represents a character or a group of character from the video content.
 * It can also be use to write notes or comments on the strip.
 * For example : NDA (note de l'adaptateur).
 */
class PhPeople {

public:
    /**
     * @brief Default constructor
     */
    PhPeople();
    /**
     * @brief PhPeople
     * @param name
     * @param color
     */
	PhPeople(QString name, QString color);
    /**
     * @brief Get the name
     * @return a string
     */
	QString getName();
    /**
     * @brief Set the name
     * @param name a string
     */
	void setName(QString name);
    /**
     * @brief Get the color
     * @return a PhColor
     */
	QString getColor();
    /**
     * @brief Set the color
     * @param color a PhColor
     */
	void setColor(QString color);

private:
    /**
     * Name of the people
     */
	QString _name;
    /**
     * Color of the people's text on the strip.
     */
	QString _color;

};

#endif // PHPEOPLE_H
