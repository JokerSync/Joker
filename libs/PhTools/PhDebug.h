/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHDEBUG_H
#define PHDEBUG_H

#include <QDebug>
#include <QDate>
#include <QRect>
#include <QFile>

#include "iostream"

#define PHDEBUG PhDebug::instance() <<  qDebug() << Q(PhDebug::getFileName(__FILE__) + "\t") << Q(PhDebug::getFuncName(__FUNCTION__) + "\t") << Q(PhDebug::getLine(__LINE__)) << "\t"
#define PHDBG(logLevelMessage) PhDebug::instance(logLevelMessage) <<  qDebug() << Q(PhDebug::getFileName(__FILE__) + "\t") << Q(PhDebug::getFuncName(__FUNCTION__) + "\t") << Q(PhDebug::getLine(__LINE__)) << "\t"




// In order to get rid of double quotes when displaying a variable
#define Q(string) (string).toStdString().c_str()

class PhDebug
{
public:
	// used to access to the only instance of the class
	static PhDebug instance(int logLevelMessage);
	static PhDebug instance();

	static PhDebug init(bool DispDate, bool DispTime, bool DispFileName, bool DispFuncName, bool DispLine, bool showConsole, int logLevel, QString appName);

	QDebug operator<<(QDebug dbg);


	PhDebug(bool DispDate, bool DispTime, bool DispFileName, bool DispFuncName, bool DispLine, bool showConsole, int logLevel, QString appName);
	static QString getFuncName(QString name);
	static QString getFileName(QString name);
	static QString getLine(int line);
	static bool isConsoleActived();
	static void writeLog(QString text);
	static void setLogLevel(int level);
	static int getLogLevel();

	static int logLevelMessage();

private:
	static PhDebug * d;
	int _logLevel;
	int _logLevelMessage;
	QFile * _log;
	QDebug * logger;
	bool _dispFuncName;
	bool _dispTime;
	bool _dispDate;
	bool _dispFileName;
	bool _dispLine;
	bool _showConsole;
	QString total;

};

#endif // PHDEBUG_H
