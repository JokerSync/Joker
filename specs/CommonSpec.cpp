/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "CommonSpec.h"

std::string f2s(PhFrame frame, PhTimeCodeType tcType)
{
	return PhTimeCode::stringFromFrame(frame, tcType).toStdString();
}

std::string t2s(PhTime time, PhTimeCodeType tcType)
{
	return PhTimeCode::stringFromTime(time, tcType).toStdString();
}

PhTime s2t(QString string, PhTimeCodeType tcType)
{
	return PhTimeCode::timeFromString(string, tcType);
}

std::string t2s25(PhTime time)
{
	return t2s(time, PhTimeCodeType25);
}

PhTime s2t25(QString string)
{
	return s2t(string, PhTimeCodeType25);
}
