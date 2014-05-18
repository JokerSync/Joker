/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHDEBUG_H
#define PHDEBUG_H

#include <QDebug>
#include <QMutex>

/** PHERR allow to log error */
#define PHERR PhDebug::error(__FILE__, __LINE__, __FUNCTION__)

/** PHDBG allow to have a multi level log system */
#define PHDBG(messageLogLevel) PhDebug::debug(__FILE__, __LINE__, __FUNCTION__, messageLogLevel)

/** PHDEBUG is the default log system */
#define PHDEBUG PHDBG(0)

/** PHNQ remove the quote when outputing a QString */
#define PHNQ(string) (string).toStdString().c_str()

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
	 * @brief Enable the log output
	 */
	static void enable();

	/**
	 * @brief Disable the log output
	 */
	static void disable();
	/**
	 * @brief Setup the debug log stream state
	 *
	 * This log stream can filter some message when the message log level is not fitting the mask.
	 *
	 * @param fileName The file name where the log was triggered from
	 * @param lineNumber The line number where the log was triggered from
	 * @param functionName The function name where the log was triggered from
	 * @param messageLogLevel The log level
	 * @return A QDebug stream
	 */
	static QDebug debug(const char *fileName, int lineNumber, const char *functionName, int messageLogLevel);

	/**
	 * @brief Setup the error log stream state
	 *
	 * @param fileName The file name where the log was triggered from
	 * @param lineNumber The line number where the log was triggered from
	 * @param functionName The function name where the log was triggered from
	 * @return A QCritical stream
	 */
	static QDebug error(const char *fileName, int lineNumber, const char *functionName);

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
	QMutex _mutex;

};

/**
 * @brief Gives human-readable event type information
 * @param stream The debug stream
 * @param event The event
 * @return The new debug stream
 */
QDebug operator <<(QDebug stream, const QEvent * event);

#endif // PHDEBUG_H
