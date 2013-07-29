#include <QCoreApplication>
#include <QDebug>

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

void display_title(char * arg)
{
    char * p = arg;
    int lg = lg_detx(arg);

    cout<<"title: ";

    for(int i=0; i<lg ; i++)
    {
        cout<<*p;
        p++;
    }
    cout<<endl;

}

int main(int argc, char *argv[])
{
 //   QCoreApplication a(argc, argv);

    bool check = verif_arg(argc);

    if (check ==false)
        return 0;

    check = verif_detx(argv[1]);

    if (check == false)
        return 0;

    display_title(argv[1]);

    return 0;
}
