/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef COMMONSPEC_H
#define COMMONSPEC_H

#include "PhSync/PhTimeCode.h"


std::string t2s(PhTime time, PhTimeCodeType tcType);
PhTime s2t(QString string, PhTimeCodeType tcType);

#endif // COMMONSPEC_H
