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


#define PHDBG(logLevelMessage) PhDebug::instance(logLevelMessage) <<  qDebug() << Q(PhDebug::getFileName(__FILE__)) << Q(PhDebug::getFuncName(__FUNCTION__)) << Q(PhDebug::getLine(__LINE__))
#define PHDEBUG PHDBG()

// In order to get rid of double quotes when displaying a variable
#define Q(string) (string).toStdString().c_str()

class PhDebug
{
public:
	// used to access to the only instance of the class
	static PhDebug instance(int logLevelMessage = 0);

	QDebug operator<<(QDebug dbg);


	PhDebug();
	static QString getFuncName(QString name);
	static QString getFileName(QString name);
	static QString getLine(int line);
	static void setLogMask(int mask);
	static int getLogMask();

	static QString logLocation();

private:
	static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

	static PhDebug * d;
	int _logMask;
	int _currentLogLevel;
	QFile * _log;
	QString _logLocation;
	bool _dispFuncName;
	bool _dispTime;
	bool _dispDate;
	bool _dispFileName;
	bool _dispLine;
	bool _showConsole;
	QString total;

};

#endif // PHDEBUG_H
