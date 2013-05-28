#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include "phstripdoc.h"
#include "PhTools/PhString.h"

int main(int argc, char *argv[])
{
    QTime *beg = new QTime();
    beg->start();


    QCoreApplication a(argc, argv);
    PhStripDoc doc("/Users/thomas/Downloads/Tunnel_Rats.detx");


    // Display infos about file
    qDebug() << "Title : " << doc.getTitle();
    qDebug() << "VideoPath : " << doc.getVideoPath();
    qDebug() << "VideoTimeStamp : " << doc.getVideoTimestamp();
    qDebug() << "FrameRate : " << doc.getFps();
    qDebug() << "DropFrame : " << doc.getDrop();
    /* disabled output for readability
    // Display characters list
    qDebug() << "characters list:";
    for(auto it : doc.getActors())
    {
        qDebug() << it->getName();
    }
    //*/

    qDebug() << "It took " << beg->elapsed() << "ms";


    exit(0);
    return a.exec();
}
