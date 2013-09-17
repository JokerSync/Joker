#ifndef TIMECODEINPUT_H
#define TIMECODEINPUT_H

#include <QWidget>
#include <QDebug>

#include "PhTools/PhTimeCode.h"


namespace Ui {
class TimeCodeInput;
}

class TimeCodeInput : public QWidget
{
    Q_OBJECT

public:
    explicit TimeCodeInput(QWidget *parent = 0);
    ~TimeCodeInput();

    void setTCType(PhTimeCodeType tcType);
    void setMediaLength(PhFrame length);

public slots:

    void isTimeCode();


signals:

    void frameChanged(PhFrame frame);

private:

    Ui::TimeCodeInput *ui;
    PhFrame _frame;
    PhFrame _medialength;
    PhTimeCodeType _tcType;



};

#endif // TIMECODEINPUT_H
