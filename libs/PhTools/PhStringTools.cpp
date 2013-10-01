#include "PhStringTools.h"
#include <math.h>

PhStringTools::PhStringTools()
{
}

QString PhStringTools::sizeToString(int size)
{
	QString stringSize;
	int oneKiloByte;

#if defined(Q_OS_MAC)
oneKiloByte = 1000;
#else
oneKiloByte = 1024;
#endif

	float floatSize;

	if(size < oneKiloByte)
		stringSize = QString::number(size) + " b";

	else
	{
		if(size < pow(oneKiloByte, 2))
		{
			floatSize = size;
			floatSize /= oneKiloByte;
			floatSize = round(floatSize * pow(10,2)) / pow(10,2);
			stringSize = QString::number(floatSize) + " Kb";
		}
		else
		{
			if(size < pow(oneKiloByte, 3))
			{
				floatSize = size;
				floatSize /= pow(oneKiloByte, 2);
				floatSize = round(floatSize * pow(10,2)) / pow(10,2);
				stringSize = QString::number(floatSize) + " Mb";
			}
			else
			{
				floatSize = size;
				floatSize /= pow(oneKiloByte, 3);
				floatSize = round(floatSize * pow(10,2)) / pow(10,2);
				stringSize = QString::number(floatSize) + " Gb";
			}
		}

	}

	return stringSize;
}
