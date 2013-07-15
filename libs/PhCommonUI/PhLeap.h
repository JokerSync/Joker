/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHLEAP_H
#define PHLEAP_H


#include <QObject>

#if LEAP
#include "Leap.h"

using namespace Leap;


class PhLeap : public QObject, public Listener
{

    Q_OBJECT

public:
    explicit PhLeap(QObject *parent = 0);
    virtual void onFrame(const Controller&);

signals:
    void setPosition(int move);
    void setRate(float move);


};
#endif
#endif // PHLEAP_H
