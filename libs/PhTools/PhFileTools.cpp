/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "QDebug"
#include "PhFileTools.h"

void copyFolder(PhString sourceFolder, PhString destFolder)
{
    QDir sourceDir(sourceFolder);
    if(!sourceDir.exists())
        return;
    QDir destDir(destFolder);
    if(!destDir.exists())
    {
        if(destDir.mkdir(destFolder))
        {
            QStringList files = sourceDir.entryList(QDir::Files);
            for(int i = 0; i< files.count(); i++)
            {
                PhString srcName = sourceFolder + "/" + files[i];
                PhString destName = destFolder + "/" + files[i];
                QFile::copy(srcName, destName);
            }
            files.clear();
            files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
            for(int i = 0; i< files.count(); i++)
            {
                PhString srcName = sourceFolder + "/" + files[i];
                PhString destName = destFolder + "/" + files[i];
                copyFolder(srcName, destName);
            }
        }
        else
        {
            qDebug() << "There's a problem while creating : " + destFolder;
        }
    }

}
