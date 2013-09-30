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

#define PHDEBUG PhDebug::instance() << qDebug() << Q(PhDebug::getFuncName(__FUNCTION__)) << Q(PhDebug::getFileName(__FILE__)) << Q(PhDebug::getLine(__LINE__)) << ":"


// In order to get rid of double quotes when displaying a variable
#define Q(string) (string).toStdString().c_str()

class PhDebug
{
public:
    // used to access to the only instance of the class
    static PhDebug instance();
    static PhDebug init(bool DispDate, bool DispTime,  bool DispFuncName, bool DispFileName, bool DispLine);

    QDebug operator<<(QDebug dbg);


    PhDebug(bool DispDate, bool DispTime, bool DispFuncName, bool DispFileName, bool DispLine);
    static QString getFuncName(QString name);
    static QString getFileName(QString name);
    static QString getLine(int line);
    static void errorHandler(QtMsgType type, const char *msg);


private:
    static PhDebug * d;
    bool _dispFuncName;
    bool _dispTime;
    bool _dispDate;
    bool _dispFileName;
    bool _dispLine;

};

#endif // PHDEBUG_H
