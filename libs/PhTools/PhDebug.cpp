/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhDebug.h"

// Global static pointer used to ensure a single instance of the class.
PhDebug* PhDebug::d = NULL;
// This function is called to create an instance of the class.
// Calling the constructor publicly is not allowed. The constructor
// is private and is only called by this Instance function.


PhDebug PhDebug::instance()
{
    if (!d)   // Only allow one instance of class to be generated.
        d = new PhDebug(true, true, true);
    return * d;
}

PhDebug PhDebug::init(bool DispDate, bool DispTime, bool DispFuncName)
{
    if (!d)   // Only allow one instance of class to be generated.
        d = new PhDebug(DispDate, DispTime, DispFuncName);
    return * d;
}

QDebug PhDebug::operator<<(QDebug dbg)
{

    QString s;

    // Display the date
    if (_dispDate)
		s = QDate::currentDate().toString("dd/MM/yyyy");

    // Display timestamp
    if (_dispTime)
        s += QTime::currentTime().toString("hh:mm:ss.zzz");

    dbg << Q(s);

    return dbg;

}

PhDebug::PhDebug(bool DispDate, bool DispTime, bool DispFuncName)
{

    if (!d){
        _dispDate = DispDate;
        _dispTime = DispTime;
        _dispFuncName = DispFuncName;
    }
}

QString PhDebug::getFuncName(QString name)
{
    if(PhDebug::instance()._dispFuncName)
        return name + ":";
    else
        return ":";
}
