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
	QByteArray localMsg = msg.toLocal8Bit();
	switch (type) {
	case QtDebugMsg:
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
		abort();
	}
	PhDebug::writeLog(msg);
}


PhDebug PhDebug::instance()
{
    if (!d)   // Only allow one instance of class to be generated.
		d = new PhDebug(true, true, true, true, true, "Default");

    return * d;
}

PhDebug PhDebug::init(bool DispDate, bool DispTime, bool DispFuncName, bool DispFileName, bool DispLine, char * name)
{
    if (!d)   // Only allow one instance of class to be generated.
		d = new PhDebug(DispDate, DispTime, DispFuncName, DispFileName, DispLine, name);
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

PhDebug::PhDebug(bool DispDate, bool DispTime, bool DispFuncName, bool DispFileName, bool DispLine, char * name)
{

	qInstallMessageHandler(myMessageOutput);
	QString appName = name;
	appName = appName.split("/").last();

	QString repo = QDir::homePath() + "/Library/Logs/Phonations/";
	if(!QDir(repo).exists()){
		PHDEBUG << repo << "doesn't exist";
		QDir().mkdir(repo);
	}
	_log = new QFile(repo + appName + ".log");
	_log->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);

	 logger = new QDebug(_log);
    //qInstallMessageHandler(errorHandler);
    if (!d){
        _dispDate = DispDate;
        _dispTime = DispTime;
        _dispFuncName = DispFuncName;
        _dispFileName = DispFileName;
        _dispLine = DispLine;
    }
}

QString PhDebug::getFuncName(QString name)
{
    if(PhDebug::instance()._dispFuncName)
        return "\"" + name + "\"";
    else
        return "";
}

QString PhDebug::getFileName(QString name)
{
    if (PhDebug::instance()._dispFileName)
        return "in " + name.split("/").last();
    return "";
}

QString PhDebug::getLine(int line)
{
    if (PhDebug::instance()._dispLine)
        return "@L" + QString::number(line);
	return "";
}

void PhDebug::writeLog(QString text)
{
	QTextStream ts(PhDebug::instance()._log);
	ts << text << endl;

}

