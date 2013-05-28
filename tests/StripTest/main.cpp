#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include "PhStrip/PhStripDoc.h"
#include "PhTools/PhString.h"

int main(int argc, char *argv[])
{
    if (argc < 2){
        qDebug() << "Please specify a file";
        return -1;
    }
    QTime *beg = new QTime();
    beg->start();


    QCoreApplication a(argc, argv);
    qDebug() << argv[1];
    PhStripDoc doc(argv[1]);


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
