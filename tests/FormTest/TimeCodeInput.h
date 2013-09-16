#ifndef TIMECODEINPUT_H
#define TIMECODEINPUT_H

#include <QWidget>

namespace Ui {
class TimeCodeInput;
}

class TimeCodeInput : public QWidget
{
    Q_OBJECT

public:
    explicit TimeCodeInput(QWidget *parent = 0);
    ~TimeCodeInput();

private:
    Ui::TimeCodeInput *ui;
};

#endif // TIMECODEINPUT_H
