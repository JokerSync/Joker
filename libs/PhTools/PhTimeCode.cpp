/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include <algorithm>
#include <QStringList>

#include "PhTimeCode.h"

#include "PhDebug.h"

QString PhTimeCode::stringFromFrame(PhFrame frame, PhTimeCodeType type) {
	unsigned int hhmmssff[4];
	ComputeHhMmSsFf(hhmmssff, frame, type);
	return QString("%1%2:%3:%4:%5").arg((frame < 0) ? "-" : "",
	                                    QString::number(hhmmssff[0]).rightJustified(2, '0'),
	                                    QString::number(hhmmssff[1]).rightJustified(2, '0'),
	                                    QString::number(hhmmssff[2]).rightJustified(2, '0'),
	                                    QString::number(hhmmssff[3]).rightJustified(2, '0'));
}

PhFrame PhTimeCode::frameFromString(QString string, PhTimeCodeType type) {
	long sign = 1;
	if ((string.length() > 0) && string.at(0) == '-') {
		sign = -1;
		string = string.remove(0, 1);
	}
	QStringList list = string.split(':');
	unsigned int hhmmssff[4];
	memset(hhmmssff, 0, 4 * sizeof(unsigned int));

	for (int i = 0; i < std::min(4, list.count()); i++) {
		int k = i;
		if(list.count() < 4)
			k += 4 - list.count();
		hhmmssff[k] = list.at(i).toInt();
	}
	return sign * frameFromHhMmSsFf(hhmmssff, type);
}

unsigned int PhTimeCode::bcdFromFrame(PhFrame frame, PhTimeCodeType type) {
	unsigned int hhmmssff[4];
	unsigned int result;
	ComputeHhMmSsFf(hhmmssff, frame, type);

	result = hhmmssff[3] % 10;
	result += (hhmmssff[3] / 10) << 4;
	result += (hhmmssff[2] % 10) << 8;
	result += (hhmmssff[2] / 10) << 12;
	result += (hhmmssff[1] % 10) << 16;
	result += (hhmmssff[1] / 10) << 20;
	result += (hhmmssff[0] % 10) << 24;
	result += (hhmmssff[0] / 10) << 28;

	return result;
}

PhFrame PhTimeCode::frameFromBcd(unsigned int bcd, PhTimeCodeType type) {
	unsigned int hhmmssff[4];

	hhmmssff[0] = (bcd >> 28) * 10;
	hhmmssff[0] += (bcd >> 24) & 0x0f;
	hhmmssff[1] = ((bcd >> 20) & 0x0f) * 10;
	hhmmssff[1] += (bcd >> 16) & 0x0f;
	hhmmssff[2] = ((bcd >> 12) & 0x0f) * 10;
	hhmmssff[2] += (bcd >> 8) & 0x0f;
	hhmmssff[3] = ((bcd >> 4) & 0x0f) * 10;
	hhmmssff[3] += bcd & 0x0f;

	return frameFromHhMmSsFf(hhmmssff, type);
}

bool PhTimeCode::isDrop(PhTimeCodeType type) {
	return type == PhTimeCodeType2997;
}

PhFrame PhTimeCode::getFps(PhTimeCodeType type) {
	switch (type) {
	case PhTimeCodeType2398:
	case PhTimeCodeType24:
		return 24;
	case PhTimeCodeType25:
		return 25;
	case PhTimeCodeType2997:
		return 30;
	}
}

PhTime PhTimeCode::timePerFrame(PhTimeCodeType type)
{
	switch (type) {
	case PhTimeCodeType2398:
		return 1001;
	case PhTimeCodeType24:
		return 1000;
	case PhTimeCodeType25:
		return 960;
	case PhTimeCodeType2997:
		return 801;
	}
}

PhTime PhTimeCode::timeFromString(QString string, PhTimeCodeType type)
{
	return frameFromString(string, type) * timePerFrame(type);
}

QString PhTimeCode::stringFromTime(PhTime time, PhTimeCodeType type)
{
	return stringFromFrame(time / timePerFrame(type), type);
}

void PhTimeCode::ComputeHhMmSsFf(unsigned int *hhmmssff, PhFrame frame, PhTimeCodeType type) {
	PhFrame fps = getFps(type);
	bool drop = isDrop(type);
	PhFrame n = qAbs(frame);

	// computing hour
	PhFrame framePerHour = 3600 * fps;
	if(drop)
		framePerHour -= 108;
	hhmmssff[0] = (unsigned int)(n / framePerHour);
	n = n % framePerHour;

	// computing tenth of minutes
	PhFrame framePerTenMinutes = 600 * fps;
	if(drop)
		framePerTenMinutes -= 18;
	hhmmssff[1] = (unsigned int)(10 * (n / framePerTenMinutes));
	n = n % framePerTenMinutes;

	// computing minutes
	PhFrame framePerMinute = 60 * fps;
	if (n >= framePerMinute) {
		hhmmssff[1] += 1;
		n -= framePerMinute;

		if (drop)
			framePerMinute -= 2;

		hhmmssff[1] += n / framePerMinute;
		n = n % framePerMinute;
	}

	// computing seconds
	PhFrame framePerSecond = fps;

	if (drop && (hhmmssff[1] % 10 > 0)) {
		if (n < framePerSecond - 2) {
			hhmmssff[2] = 0;
			n += 2;
		}
		else {
			hhmmssff[2] = (unsigned int)(1 + (n - framePerSecond + 2) / framePerSecond);
			n = (n - framePerSecond + 2) % framePerSecond;
		}
	}
	else {
		hhmmssff[2] = (unsigned int)(n / framePerSecond);
		n = n % framePerSecond;
	}

	hhmmssff[3] = (unsigned int)n;
}

PhFrame PhTimeCode::frameFromHhMmSsFf(unsigned int *hhmmssff, PhTimeCodeType type) {
	PhFrame fps = getFps(type);

	if (hhmmssff[1] >= 60) {
		PHDEBUG << "Bad minute value:" << QString::number(hhmmssff[1]);
		hhmmssff[1] = 0;
	}
	if (hhmmssff[2] >= 60) {
		PHDEBUG << "Bad second value:" << QString::number(hhmmssff[2]);
		hhmmssff[2] = 0;
	}
	if (hhmmssff[3] >= fps) {
		PHDEBUG << "Bad frame value:" << QString::number(hhmmssff[3]);
		hhmmssff[3] = 0;
	}
	PhFrame dropframe = 0;
	if (isDrop(type)) {
		// counting drop per hour
		dropframe += hhmmssff[0] * 108;
		// counting drop per tenth of minute
		dropframe += (hhmmssff[1] / 10) * 18;
		// counting drop per minute
		dropframe += (hhmmssff[1] % 10) * 2;
	}
	return (((hhmmssff[0] * 60) + hhmmssff[1]) * 60 + hhmmssff[2]) * fps + hhmmssff[3] - dropframe;
}
