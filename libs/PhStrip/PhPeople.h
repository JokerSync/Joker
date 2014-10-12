/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHPEOPLE_H
#define PHPEOPLE_H

#include <QObject>
#include <QString>

/**
 * @brief Represents a character or a group of character from the video content.
 *
 * It can also be use to write notes or comments on the strip.
 * For example : NDA (note de l'adaptateur).
 */
class PhPeople : public QObject {

	Q_OBJECT

	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

public:
	/**
	 * @brief PhPeople
	 * @param name
	 * @param color
	 */
	PhPeople(QString name = "???", QString color = "#000000");
	/**
	 * @brief Get the name
	 * @return a string
	 */
	QString name() const;
	/**
	 * @brief Set the name
	 * @param name a string
	 */
	void setName(QString name);
	/**
	 * @brief Get the color
	 * @return a PhColor
	 */
	QString color() const;
	/**
	 * @brief Set the color
	 * @param color a PhColor
	 */
	void setColor(QString color);

signals:
	void nameChanged();
	void colorChanged();

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
