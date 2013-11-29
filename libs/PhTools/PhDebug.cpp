/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhDebug.h"

#include <QStringList>
#include <QFile>
#include <QDir>

PhDebug* PhDebug::d = NULL;

// This function is called to create an instance of the class.
// Calling the constructor publicly is not allowed. The constructor
// is private and is only called by this Instance function.


void PhDebug::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	if(d)
	{
		if(d->_logMask & (1 << d->_currentLogLevel))
		{
			if(d->_showConsole)
				fprintf(stderr, "%s \n", msg.toLocal8Bit().constData());
			QTextStream ts(d->_log);
			ts << msg << endl;
		}
	}
	else
		fprintf(stderr, "%s \n", msg.toLocal8Bit().constData());
}
QString PhDebug::logLocation()
{
	return d->_logLocation;
}


// Called if init() was forget
PhDebug PhDebug::instance(int logLevelMessage)
{
    if (!d)   // Only allow one instance of class to be generated.
        d = new PhDebug();

	d->_currentLogLevel = logLevelMessage;
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

PhDebug::PhDebug()
{
	qInstallMessageHandler(this->messageOutput);

	QString logDirPath = QDir::homePath() + "/Library/Logs/Phonations/";
	QDir logDir(logDirPath);
	if(!logDir.exists()) {
		QDir().mkdir(logDirPath);
	}
	_log = new QFile(logDirPath + APP_NAME + ".log");
	_logLocation = _log->fileName();
	_log->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	_log->write("\n\n");

	_dispDate = false;
	_dispTime = true;
	_dispFuncName = true;
	_dispFileName = true;
	_dispLine = true;
	_showConsole = true;
	_logMask = 1;
}

QString PhDebug::getFuncName(QString name)
{
	if(d && d->_dispFuncName)
		return name + "\t";
	else
		return "";
}

QString PhDebug::getFileName(QString name)
{
	if (d && d->_dispFileName)
		return  name.split("/").last() + "\t";
	return "";
}

QString PhDebug::getLine(int line)
{
	if (d && d->_dispLine)
		return "@L" + QString::number(line) + "\t";
	return "";
}

void PhDebug::setLogMask(int mask)
{
	if (d)
		d->_logMask = mask;
}

int PhDebug::getLogMask()
{
	return d->_logMask;
}



