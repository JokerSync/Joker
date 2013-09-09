/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHDEBUG_H
#define PHDEBUG_H

#include <QDebug>
#include <QDate>
#include <QRect>

#include "iostream"

#define PHDEBUG PhDebug::instance() << qDebug() << __FUNCTION__ << ":"


// In order to get rid of double quotes when displaying a variable
#define Q(string) (string).toStdString().c_str()

class PhDebug
{
public:
    // used to access to the only instance of the class
    static PhDebug instance();
    static PhDebug init(bool DispTime, bool DispFuncName);

    QDebug operator<<(QDebug dbg)
    {
        QString s;

        if (_time){
            s = QDate::currentDate().toString("dd.MM.yyyy");
            s += " - ";
            s += QTime::currentTime().toString("hh.mm.ss.zzz");
            s += " ";
        }
        if (_fname && !_time){
            s += "In";
        }
        if(_fname && _time){
            s += "in";
        }
        dbg << Q(s);

        return dbg;
    }

    PhDebug(bool DispTime, bool DispFuncName);


private:
    static PhDebug * d;
    bool _fname;
    bool _time;

};

#endif // PHDEBUG_H
