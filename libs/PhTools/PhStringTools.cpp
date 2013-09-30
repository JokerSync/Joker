#include "PhStringTools.h"
#include <math.h>

PhStringTools::PhStringTools()
{
}

QString PhStringTools::sizeToString(int size)
{
	QString stringSize;
	float floatSize;

	if(size < pow(10,3))
		stringSize = QString::number(size) + " b";

	else
	{
		if(size < pow(10,6))
		{
			floatSize = size;
			floatSize /= pow(10,3);
			floatSize = round(floatSize * pow(10,2)) / pow(10,2);
			stringSize = QString::number(floatSize) + " Kb";
		}
		else
		{
			if(size < pow(10,9))
			{
				floatSize = size;
				floatSize /= pow(10,6);
				floatSize = round(floatSize * pow(10,2)) / pow(10,2);
				stringSize = QString::number(floatSize) + " Mb";
			}
			else
			{
				floatSize = size;
				floatSize /= pow(10,9);
				floatSize = round(floatSize * pow(10,2)) / pow(10,2);
				stringSize = QString::number(floatSize) + " Gb";
			}
		}

	}

	return stringSize;
}
