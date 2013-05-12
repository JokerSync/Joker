#ifndef DWWINDOW_H
#define DWWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include "testsdlwidget.h"

namespace Ui {
class DWwindow;
}

class DWwindow : public QMainWindow
{
    Q_OBJECT

public:
    //~DWwindow();

    DWwindow(int width, int height);

    void keyPressEvent(QKeyEvent *keyEvent);

private:
    TestSDLWidget * testSDLWidget;

};
#endif // DWWINDOW_H
