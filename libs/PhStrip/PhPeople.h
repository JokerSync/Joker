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
	 * @brief PhPeople constructor
	 * @param name The people name
	 * @param color The people color
	 * @param picture The people picture
	 */
	PhPeople(QString name = "???", QString color = "#000000", QString picture = "");
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
	/**
	 * @brief The picture in base64 coding
	 * @return A base64 string containing JPEG data.
	 */
	QString picture() const;
	/**
	 * @brief Set the picture date
	 * @param picture A base64 string containing JPEG data.
	 */
	void setPicture(QString picture);

signals:
	void nameChanged();
	void colorChanged();

private:
	QString _name;
	QString _color;
	QString _picture;

};

#endif // PHPEOPLE_H
