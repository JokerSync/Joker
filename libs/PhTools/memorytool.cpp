/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/
#include <QDebug>
#include <QString>

#include "memorytool.h"


void MemoryDump(void * pixels, int w, int h, int bpp)
{
    for(int i=0;i<h;i++)
    {
        QString s = "";
        for(int j=0;j<w;j++)
        {
            int pixel = 0;
            for(int k = 0;k<bpp;k++)
            {
                unsigned char * p = (unsigned char*)pixels + (i* w + j)* bpp + k;
                pixel += *p;
            }
            if(pixel > bpp * 128)
                s += "X";
            else
                s += "_";
        }
        PHDEBUG << s;
    }
}

