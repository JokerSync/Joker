#include <QApplication>
#include <QDebug>
#include <QTime>

#include "SDL/SDL.h"

#include "PhStrip/PhStripDoc.h"
#include "PhTools/PhString.h"

#include "StripWindow.h"

int main(int argc, char *argv[])
{
    if (argc < 2){
        qDebug() << "Please specify a file";
        return -1;
    }
    QTime *beg = new QTime();
    beg->start();




    QApplication a(argc, argv);
    StripWindow mainwindow(NULL, argv[1]);
    mainwindow.show();


    /* disabled output for readability
    // Display characters list
    qDebug() << "characters list:";
    for(auto it : doc.getActors())
    {
        qDebug() << it->getName();
    }
    //*/

    /* disabled output for readability
    // Display characters list
    qDebug() << "\n" <<"text list:";
    for(auto it : doc.getTexts())
    {
        qDebug() << it->getPeople().getName() << ":" << it->getContent();
    }
    //*/
    qDebug() << "It took " << beg->elapsed() << "ms";


    return a.exec();
}
