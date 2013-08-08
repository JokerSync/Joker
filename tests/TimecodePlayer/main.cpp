#include "mainwindow.h"
#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QWidget *fenetre = new QWidget;
    fenetre->resize(600,200);
    fenetre->setLayout(w.getLayout());
    fenetre->show();
    /*w.setLayout(w.getLayout());
    w.show();*/
    
    return a.exec();
}
