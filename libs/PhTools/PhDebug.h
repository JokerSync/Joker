/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHDEBUG_H
#define PHDEBUG_H

#include <QDebug>

/** PHDBG allow to have a multi level log system */
#define PHDBG(messageLogLevel) PhDebug::debug(messageLogLevel)

/** PHDEBUG is the default log system */
#define PHDEBUG PHDBG()

/**
 * @brief A custom log system
 *
 * It provides a powerful log tool, using mask to show/hide some (un)desired
 * log informations which can be saved - or not - to a local file.
 */
class PhDebug
{
public:
	/**
	 * @brief
	 * @param messageLogLevel
	 * @return
	 */
	static QDebug debug(int messageLogLevel = 0);

	/**
	 * @brief Set the log mask
	 *
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

	/**
	 * @brief Specify what a log message shall display
	 * @param date Display the date
	 * @param time Display the time
	 * @param fileName Display the file name
	 * @param functionName Display the function name
	 * @param line Display the line number
	 */
	static void setDisplay(bool date, bool time, bool fileName, bool functionName, bool line);
private:
	/**
	 * @brief PhDebug constructor
	 */
	PhDebug();

	/**
	 * @brief Get the singleton instance of PhDebug
	 *
	 * @return The debugger
	 */
	static PhDebug* instance();

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
