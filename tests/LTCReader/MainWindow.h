#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "PhCommonUI/PhMediaPanel.h"
#include "PhTools/PhClock.h"
#include "PhSync/PhLtcReader.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *_timer;
    PhClock *_clock;
    PhLtcReader _reader;
};

#endif // MAINWINDOW_H
