/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhDebug.h"

#include <QStringList>

// Global static pointer used to ensure a single instance of the class.
PhDebug* PhDebug::d = NULL;
// This function is called to create an instance of the class.
// Calling the constructor publicly is not allowed. The constructor
// is private and is only called by this Instance function.


PhDebug PhDebug::instance()
{
    if (!d)   // Only allow one instance of class to be generated.
        d = new PhDebug(true, true, true, true, true);
    return * d;
}

PhDebug PhDebug::init(bool DispDate, bool DispTime, bool DispFuncName, bool DispFileName, bool DispLine)
{
    if (!d)   // Only allow one instance of class to be generated.
        d = new PhDebug(DispDate, DispTime, DispFuncName, DispFileName, DispLine);
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

    dbg << Q(s + ":");

    return dbg;

}

PhDebug::PhDebug(bool DispDate, bool DispTime, bool DispFuncName, bool DispFileName, bool DispLine)
{
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


//Unused at this moment.
void errorHandler(QtMsgType type, const char *msg)
{
    switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "%s\n", msg);
            break;
        case QtWarningMsg:
            fprintf(stderr, "\033[1;33mWarning\033[0m: %s\n", msg);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "\033[31mCritical\033[0m: %s\n", msg);
            break;
        case QtFatalMsg:
            fprintf(stderr, "\033[31mFatal\033[0m: %s\n", msg);
            abort();
    }
}
