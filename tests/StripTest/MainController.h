/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QSettings>
#include <QDebug>
#include <QDir>

#include "PhTools/PhString.h"

class MainWindow;

class MainController
{
public:
    MainController(MainWindow * mw);
    MainController();
    void loadSettings();
    PhString getLastFile();
private:
    MainWindow * _window;
    QSettings * _settings;

};

#endif // MAINCONTROLLER_H
