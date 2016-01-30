/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef COMMONSPEC_H
#define COMMONSPEC_H

#include <QTest>
#include "bandit/bandit.h"

#include "PhSync/PhTimeCode.h"


QString f2s(PhFrame frame, PhTimeCodeType tcType);

QString t2s(PhTime time, PhTimeCodeType tcType);

PhTime s2t(QString string, PhTimeCodeType tcType);

QString t2s25(PhTime time);

PhTime s2t25(QString string);

bool operator==(const std::string& str, const QString& qstr);

#endif // COMMONSPEC_H
