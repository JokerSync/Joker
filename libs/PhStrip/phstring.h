#ifndef PHSTRING_H
#define PHSTRING_H

#include <QString>

class PhString
{
public:
    PhString(QString str);
    PhString(std::string str);
};

#endif // PHSTRING_H
