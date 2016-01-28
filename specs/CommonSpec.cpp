/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <qmath.h>
#include <QColor>

#include "CommonSpec.h"
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

std::string t2s25(PhTime time)
{
	return t2s(time, PhTimeCodeType25);
}

PhTime s2t25(QString string)
{
	return s2t(string, PhTimeCodeType25);
}

bool compareImage(QImage result, QImage expected, QString testName, int gap)
{
	bool ret = false;
	if((result.width() == 0) || (result.height() == 0)) {
		PHDBG(5) << testName << " : bad result image size";
	}
	else if((expected.width() == 0) || (expected.height() == 0)) {
		PHDBG(5) << testName << " : bad result image size";
	}
	else if(result.width() != expected.width()) {
		PHDBG(5) << testName << " : different width: " << result.width() << " / " << expected.height();
	}
	else {
//		if(gap == 0) {
//			ret = (result == expected); // doesn't work alway
//		}
//		else {
			int totalDiff = 0;
			ret = true;
			for(int i = 0; i < result.width(); i++) {
				for(int j = 0; j < result.height(); j++) {
					QRgb a = result.pixel(i, j);
					QRgb b = expected.pixel(i, j);
					int diff = qPow(qRed(a) - qRed(b), 2) + qPow(qGreen(a) - qGreen(b), 2) + qPow(qBlue(a) - qBlue(b), 2);
					if(diff)
						PHDBG(5) << QString("(%1, %2) %3 / %4 => %5 / %6")
									.arg(i)
									.arg(j)
									.arg(QColor(a).name())
									.arg(QColor(b).name())
									.arg(diff)
									.arg(totalDiff);
					totalDiff += diff;
					if(totalDiff > std::numeric_limits<int>::max() / 2) {
						PHDBG(5) << QString("(%1, %2) Too many difference detected: %3")
									.arg(i)
									.arg(j)
									.arg(totalDiff);
						ret = false;
						break;
					}
					if(totalDiff > gap) {
						PHDBG(5) << QString("(%1, %2) Too many difference detected: %3 > %4")
									.arg(i)
									.arg(j)
									.arg(totalDiff)
									.arg(gap);
						ret = false;
						break;
					}
				}

				if(!ret)
					break;
		//	}
		}

		if(!ret) {
			result.save(testName + "_result.bmp");
			PHDBG(5) << testName << " : different content";
		}

	}

	return ret;
}
