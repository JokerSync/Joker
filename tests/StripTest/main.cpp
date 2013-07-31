
#include <QCoreApplication>
#include <QDebug>

#include "QMap"
#include "QList"

#include "PhStrip/PhStripDoc.h"

#include <iostream>

using namespace std;


bool verif_detx(char * arg)
{
      char * p = arg;

      while (*p != '\0')
      {
          p++;
      }

      int i = 0;

      for(i=0; i<5; i++)
      {
          p--;
      }

      if(*p != '.')
      {
          qDebug()<<"Not a detx file";
          return false;
      }
      p++;

      if(*p != 'd')
      {
          qDebug()<<"Not a detx file";
          return false;
      }
      p++;

      if(*p != 'e')
      {
          qDebug()<<"Not a detx file";
          return false;
      }
      p++;

      if(*p != 't')
      {
          qDebug()<<"Not a detx file";
          return false;
      }
      p++;

      if(*p != 'x')
      {
          qDebug()<<"Not a detx file";
          return false;
      }

      qDebug()<<"Il s'agit bien d'un fichier detx";
      return true;

}

int main(int argc, char *argv[])
{
    //Check if DetX file path as argument

    if (argc == 1)
    {
        qDebug()<<"Please provide a DetX file path as argument";
        return 0;
    }

    if (argc > 2)
    {
        qDebug()<<"too much arguments";
        return 0;
    }

    //Check if

    check = verif_detx(argv[1]);

    if (check == false)
        return 0;

    // Creating a new doc:
    PhStripDoc doc;

    // Open the DetX file in argument:
    PhString fileName(argv[1]);
    check = doc.openDetX(fileName);

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
    PhString line;

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
