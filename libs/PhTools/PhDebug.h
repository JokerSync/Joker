/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHDEBUG_H
#define PHDEBUG_H

#include <QDebug>
#include <QDate>
#include <QRect>

#define DEBUG PhDebug d; d <<  PHDEBUG

#define PHDEBUG  qDebug() << __FUNCTION__ << ":"

// In order to get rid of double quotes when displaying a variable
#define Q(string) (string).toStdString().c_str()

class PhDebug
{
public:
    QDebug operator<<(QDebug dbg)
    {
        QString d;
        d = QDate::currentDate().toString("dd.MM.yyyy");
        d += " - ";
        d += QTime::currentTime().toString("hh.mm.ss.zzz");
        d += " in";
        dbg << Q(d);

        return dbg;
    }

};

#endif // PHDEBUG_H
