#include "memorytool.h"
#include <QDebug>
#include <QString>

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
                char * p = (char*)pixels + (i* w + j)* bpp + k;
                pixel += *p;
            }
            if(pixel > 0)
                s += "X";
            else
                s += "_";
        }
        qDebug() << s;
    }
}

