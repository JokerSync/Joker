/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H


class MainWindow;

class MainController
{
public:
    MainController(MainWindow * mw);
    MainController();
private:
    MainWindow * _window;
};

#endif // MAINCONTROLLER_H
