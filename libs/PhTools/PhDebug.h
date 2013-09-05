/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHDEBUG_H
#define PHDEBUG_H

#include <QDebug>
#include <QDate>
#include <QRect>


#define PHDEBUG PhDebug::instance() << qDebug() << __FUNCTION__ << ":"


// In order to get rid of double quotes when displaying a variable
#define Q(string) (string).toStdString().c_str()

class PhDebug
{
public:
    // used to access to the only instance of the class
    static PhDebug * instance();

    QDebug operator<<(QDebug dbg)
    {
        QString s;
        s = QDate::currentDate().toString("dd.MM.yyyy");
        s += " - ";
        s += QTime::currentTime().toString("hh.mm.ss.zzz");
        s += " in";
        dbg << Q(s);

        return dbg;
    }

private:
    static PhDebug * d;
    PhDebug(){}

};

#endif // PHDEBUG_H
