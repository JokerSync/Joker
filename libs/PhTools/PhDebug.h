/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHDEBUG_H
#define PHDEBUG_H

#include <QDebug>
#include <QDate>
#include <QRect>
#include <QFile>

#include "iostream"

/** PHDBG allow to have a multi level log system */
#define PHDBG(logLevelMessage) PhDebug::setCurrentLogLevel(logLevelMessage)

/** PHDEBUG is the default log system */
#define PHDEBUG PHDBG(0)

/**
 * @brief A custom log system
 *
 * It provides a powerful log tool, using mask to show/hide some (un)desired
 * log informations which can be saved - or not - to a local file.
 */
class PhDebug
{
public:

	PhDebug();

	/**
	 * @brief Get the instance
	 * As the debugger is a singleton, this is the only way to access it.
	 * @param logLevelMessage the desired mask
	 * @return The debugger
	 */
	static PhDebug* instance();

	static QDebug setCurrentLogLevel(int messageLogLevel);

	/**
	 * @brief Get the function name
	 * @param name The name of the function
	 * @return If the output is enabled, the function name, else an empty string
	 */
	static QString getFuncName(QString name);
	/**
	 * @brief Get the file name
	 * @param name The name of the file
	 * @return If the output is enabled, the file name, else an empty string
	 */
	static QString getFileName(QString name);
	/**
	 * @brief Get the line number
	 * @param line The name of the function
	 * @return If the output is enabled, the number of the line, else an empty string
	 */
	static QString getLine(int line);
	/**
	 * @brief Set the log mask
	 * Enable the message with the corresponding mak :
	 *
	 *	Always			=>	0
	 *	Custom			=>	1-7
	 *	Graphic			=>	8-11
	 *	GraphicStrip	=>	12-15
	 *	Strip			=>	16-19
	 *  Sync			=>	20-23
	 *  Video			=>	24-27
	 *	CommonUI		=>	28-31
	 *
	 * @param mask The desired mask
	 *
	 * For exemple if mask = 0b1110000001111111 (57471)
	 *
	 * It the debugger will show the Always, Customs and Strip messages
	 */
	static void setLogMask(int mask);
	/**
	 * @brief Get the current log mask
	 * @return The log mask.
	 */
	static int getLogMask();
	/**
	 * @brief Get the log location
	 * As the log file is with the others system & user logs, and this place
	 * is plateform dependent, it's useful to be able to tell the users where
	 * to find the log file.
	 * @return The url of the log file
	 */
	static QString logLocation();

	/**
	 * @brief show log in console
	 * @param show enable or disable the console log.
	 */
	static void showConsole(bool show);

	static void setDisplay(bool date, bool time, bool fileName, bool functionName, bool line);
private:
	/**
	 * @brief Custom message output
	 *
	 * This function is called to create an instance of the class.
	 * Calling the constructor publicly is not allowed. The constructor
	 * is private and is only called by this Instance function.
	 *
	 * @param type The message type
	 * @param context The message context
	 * @param msg The message
	 */
	static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

	static PhDebug * _d;
	int _logMask;
	int _currentLogLevel;
	QTextStream *_textLog;
	QString _logFileName;
	bool _displayDate;
	bool _displayTime;
	bool _displayFileName;
	bool _displayFunctionName;
	bool _displayLine;
	bool _showConsole;
	QString total;

};

#endif // PHDEBUG_H
