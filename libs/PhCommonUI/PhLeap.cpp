/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QDebug>
#include "PhLeap.h"

using namespace Leap;

void PhLeap::onFrame(const Controller& controller)
{
    qDebug() << "on frame";
}
