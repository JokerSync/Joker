/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHLEAP_H
#define PHLEAP_H


#include "Leap.h"

using namespace Leap;

class PhLeap : public Listener
{
public:
    virtual void onFrame(const Controller&);


};

#endif // PHLEAP_H
