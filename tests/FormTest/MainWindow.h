#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

#include "PhTools/PhTimeCode.h"
#include "PhCommonUI/TimeCodeEdit.h"

namespace Ui {
class MainWindow;
}

class Dialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void updateText(PhFrame frame, PhTimeCodeType tcType);

private slots:
    void on_actionGoto_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    PhFrame _frame;
    PhTimeCodeType _tcType;
    Dialog *_goto;
    Dialog *_about;
};

#endif // MAINWINDOW_H
