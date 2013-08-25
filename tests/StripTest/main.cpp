
#include <QCoreApplication>
#include <QDebug>

#include "QMap"
#include "QList"

#include "PhStrip/PhStripDoc.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    //Check argument count
    if (argc < 2)
    {
        qDebug() << "Please provide a DetX file path as argument";
        return 0;
    }

    //Check if it's a DetX file
    QFileInfo file (argv[1]);
    QString ext = file.suffix();

    if(ext != "detx")
    {
        qDebug() << "It's not a DetX file";
        return 0;
    }

    // Creating a new doc:
    PhStripDoc doc;

    // Open the DetX file in argument:
	QString fileName(argv[1]);
    doc.openDetX(fileName);

    // Display the title:

    qDebug() << "title : " ;
    qDebug() <<doc.getTitle();
    qDebug() <<"-----------------------------";

    // Display actors

    QMap<QString, PhPeople *> list_actors = doc.getActors();
    qDebug() << "actors : ";
    QMap<QString, PhPeople *>::iterator it;

    for( it=list_actors.begin(); it!=list_actors.end() ; it++)
    {
        qDebug() << (*it)->getName();
    }

     qDebug() <<"-----------------------------";


    // Display text

    QList<PhStripText *>textList = doc.getTexts();
    qDebug() << "texts : ";
    QList<PhStripText *>::iterator text;
	QString line;

    for( text = textList.begin(); text != textList.end() ; text++)
    {
          if(text == textList.begin())
          {
              line = (*text)->getPeople().getName();
              line += " : ";
              line += (*text)->getContent();
          }
          else
          {

             if(((*text)->getPeople().getName()) != ((*(text-1))->getPeople().getName()))
             {
                 qDebug() << qPrintable( line );
                 line = (*text)->getPeople().getName();
                 line += " : ";
                 line += (*text)->getContent();
             }
             else
             {
                 line += (*text)->getContent();
             }
          }
    }
     qDebug() << qPrintable( line );

    return 0;
}
