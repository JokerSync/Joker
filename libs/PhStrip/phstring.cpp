#include "phstring.h"

PhString::PhString(QString str)
{
    return str;
}

PhString::PhString(std::string str)
{
    return QString(str);
}
