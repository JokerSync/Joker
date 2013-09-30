#include "PhStringTools.h"
#include <math.h>

PhStringTools::PhStringTools()
{
}

QString PhStringTools::sizeToString(int size)
{
	QString stringSize;
	float floatSize;

	if(size < 1<<10)
		stringSize = QString::number(size) + " b";

	else
	{
		if(size < 1<<20)
		{
			floatSize = size;
			floatSize /= 1<<10;
			floatSize = round(floatSize * pow(10,2)) / pow(10,2);
			stringSize = QString::number(floatSize) + " Kb";
		}
		else
		{
			if(size < 1<<30)
			{
				floatSize = size;
				floatSize /= 1<<20;
				floatSize = round(floatSize * pow(10,2)) / pow(10,2);
				stringSize = QString::number(floatSize) + " Mb";
			}
			else
			{
				floatSize = size;
				floatSize /= 1<<30;
				floatSize = round(floatSize * pow(10,2)) / pow(10,2);
				stringSize = QString::number(floatSize) + " Gb";
			}
		}

	}

	return stringSize;
}
