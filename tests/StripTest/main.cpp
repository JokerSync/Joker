#include <QCoreApplication>
#include <QDebug>

#include "PhStrip/PhStripDoc.h"

#include <iostream>

using namespace std;

bool verif_arg(int nb)
{
    if (nb == 1)
    {
        qDebug()<<"not enough arguments";
        return false;
    }

    if (nb > 2)
    {
        qDebug()<<"too much arguments";
        return false;
    }

    return true;
}

int lg_arg(char * arg)
{
    char *p = arg;
    int cpt = 0;
    while (*p != '\0')
    {
        p++;
        cpt++;
    }

    return cpt;
}

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

int lg_detx(char * arg)
{
    char *p = arg;
    int cpt = 0;
    while (*p != '\0')
    {
        p++;
        cpt++;
    }

    cpt = cpt-5;
    return cpt;
}

int main(int argc, char *argv[])
{
    bool check = verif_arg(argc);

    if (check ==false)
    {
        cout << "Please provide a DetX file path as argument" << endl;
        return 0;
    }

    check = verif_detx(argv[1]);

    if (check == false)
        return 0;

    // Creating a new doc:
    PhStripDoc doc;

    // Open the DetX file in argument:
    QString fileName(argv[1]);
    check = doc.openDetX(fileName);

    // Display the title:
    qDebug() << doc.getTitle();

    return 0;
}
