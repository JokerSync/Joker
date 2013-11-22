/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhDebug.h"

#include <QStringList>
#include <QFile>
#include <QDir>

// Global static pointer used to ensure a single instance of the class.
PhDebug* PhDebug::d = NULL;
// This function is called to create an instance of the class.
// Calling the constructor publicly is not allowed. The constructor
// is private and is only called by this Instance function.


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

	if(PhDebug::instance().getLogLevel() & PhDebug::instance().logLevelMessage() ){
		QByteArray localMsg = msg.toLocal8Bit();
		switch (type) {
		case QtDebugMsg:
			if(PhDebug::isConsoleActived())
				fprintf(stderr, "%s \n", localMsg.constData());
			break;
		case QtWarningMsg:
			fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			PhDebug::writeLog(msg);
			abort();
		}
		PhDebug::writeLog(msg);
	}
}


// Called if init() was forget
PhDebug PhDebug::instance(int logLevelMessage)
{
	if (!d){   // Only allow one instance of class to be generated.
		d = new PhDebug(false, true, true, true, true, true, 16, "Default");
		//Display two white lines at program start

		PhDebug::writeLog("\n");
	}
	d->_logLevelMessage = logLevelMessage;
	return * d;
}

PhDebug PhDebug::instance()
{
	if (!d){   // Only allow one instance of class to be generated.
		d = new PhDebug(false, true, true, true, true, true, 16,  "Default");
		//Display two white lines at program start

		PhDebug::writeLog("\n");
	}
	d->_logLevelMessage = 0x11111111;
	return * d;
}

PhDebug PhDebug::init(bool DispDate, bool DispTime, bool DispFileName, bool DispFuncName, bool DispLine, bool showConsole, int logLevel, QString appName)
{
	if (!d){  // Only allow one instance of class to be generated.
		d = new PhDebug(DispDate, DispTime, DispFileName, DispFuncName, DispLine, showConsole, logLevel, appName);
		//Display two white lines at program start
		PhDebug::writeLog("\n");
	}
	return * d;
}

QDebug PhDebug::operator<<(QDebug dbg)
{

	QString s;

	// Display the date
	if (_dispDate)
		s = QDate::currentDate().toString("dd/MM/yyyy");

	if (_dispDate && _dispTime)
		s += " ";

	// Display timestamp
	if (_dispTime)
		s += QTime::currentTime().toString("hh:mm:ss.zzz");


	dbg << Q(s);

	return dbg;

}

PhDebug::PhDebug(bool DispDate, bool DispTime, bool DispFileName, bool DispFuncName, bool DispLine, bool showConsole, int logLevel, QString appName)
{
	qInstallMessageHandler(myMessageOutput);

	QString logDirPath = QDir::homePath() + "/Library/Logs/Phonations/";
	QDir logDir(logDirPath);
	if(!logDir.exists()) {
		QDir().mkdir(logDirPath);
	}
	_log = new QFile(logDirPath + appName + ".log");
	_log->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);

	logger = new QDebug(_log);

	if (!d){
		_dispDate = DispDate;
		_dispTime = DispTime;
		_dispFuncName = DispFuncName;
		_dispFileName = DispFileName;
		_dispLine = DispLine;
		_showConsole = showConsole;
		_logLevel = logLevel;
	}

}

QString PhDebug::getFuncName(QString name)
{
	if(PhDebug::instance()._dispFuncName)
		return name;
	else
		return "";
}

QString PhDebug::getFileName(QString name)
{
	if (PhDebug::instance()._dispFileName)
		return  name.split("/").last();
	return "";
}

QString PhDebug::getLine(int line)
{
	if (PhDebug::instance()._dispLine)
		return "@L" + QString::number(line);
	return "";
}

bool PhDebug::isConsoleActived()
{
	return PhDebug::instance()._showConsole;
}

void PhDebug::writeLog(QString text)
{
	QTextStream ts(PhDebug::instance()._log);
	ts << text << endl;
}

#warning TODO remove if useless
void PhDebug::setLogLevel(int level)
{
	d->_logLevel = level;
}

int PhDebug::getLogLevel()
{
	return d->_logLevel;
}
int PhDebug::logLevelMessage()
{
	return d->_logLevelMessage;
}



