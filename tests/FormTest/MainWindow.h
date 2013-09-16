#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "PhTools/PhTimeCode.h"
#include "TimeCodeInput.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void updateText(PhFrame frame);

private:
    Ui::MainWindow *ui;
    PhFrame _frame;
    PhTimeCodeType _tcType;

};

#endif // MAINWINDOW_H
