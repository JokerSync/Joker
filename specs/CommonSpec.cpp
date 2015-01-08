/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "CommonSpec.h"
#include "PhTools/PhData.h"
#include "PhTools/PhDebug.h"

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

bool compareImage(QImage result, QImage expected, QString testName)
{
	if((result.width() == 0) || (result.height() == 0)) {
		PHDBG(5) << testName << " : bad result image size";
		return false;
	}

	if((expected.width() == 0) || (expected.height() == 0)) {
		PHDBG(5) << testName << " : bad result image size";
		return false;
	}

	if(result.width() != expected.width()) {
		PHDBG(5) << testName << " : different width: " << result.width() << " / " << expected.height();
		return false;
	}

	if(result != expected) {
		PHDBG(5) << testName << " : different content";
		result.save(testName + "_result.bmp");
		return false;
	}

	return true;
}
