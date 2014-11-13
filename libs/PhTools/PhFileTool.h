/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHFILETOOL_H
#define PHFILETOOL_H

#include "PhTools/PhFile.h"

/**
 * @brief Provides tools for binary files reading
 */
class PhFileTool
{
public:
	/**
	 * @brief Read a single char
	 * @param f The file
	 * @param level The log level
	 * @param name The name (for logging purpose)
	 * @return A char
	 */
	static unsigned char readChar(QFile &f, int level, QString name = "???");

	/**
	 * @brief Read a two-bytes unsigned short
	 * @param f The file
	 * @param level The log level
	 * @param name The name (for logging purpose)
	 * @return An unsigned short
	 */
	static unsigned short readShort(QFile &f, int level, QString name = "???");

	/**
	 * @brief Read a four-bytes signed integer
	 * @param f The file
	 * @param level The log level
	 * @param name The name (for logging purpose)
	 * @return A signed int
	 */
	static int readInt(QFile &f, int level, QString name = "???");

	/**
	 * @brief Read a string
	 *
	 * The readString method assumes that the string starts with its size
	 * and is followed by the character data.
	 * If the first char is 0xFF, the next char will contain the string size
	 * and a wide char string will be decoded.
	 * @param f The file
	 * @param level The log level
	 * @param name The name (for logging purpose)
	 * @return A string
	 */
	static QString readString(QFile &f, int level, QString name = "???");
};

#endif // PHFILETOOL_H
