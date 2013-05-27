#include <QCoreApplication>
#include <QDebug>
#include "phstripdoc.h"
#include "PhTools/PhString.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PhStripDoc doc("/Users/thomas/Downloads/Tunnel_Rats.detx");

    /*
    // Display infos about file
    qDebug() << "Title : " << doc.getTitle();
    qDebug() << "VideoPath : " << doc.getVideoPath();
    qDebug() << "VideoTimeStamp : " << doc.getVideoTimestamp();
    qDebug() << "FrameRate : " << doc.getFps();
    qDebug() << "DropFrame : " << doc.getDrop();
    //qDebug() << "characters list:";
    */

    exit(0);
    return a.exec();
}
