#include "phdebug.h"
#include "QDebug"

using namespace test;

void PhDebug(QString msg)
{

    PHDEBUG << test::Audio;
    PHDEBUG <<  test::DoCommand;
    PHDEBUG <<  test::LTC;
    PHDEBUG <<  test::LTC;
    PHDEBUG <<  test::Clock;
    PHDEBUG <<  test::Sony;

    PHDEBUG << "@" + QString::number(LTC) + ": " + msg;
    QtMsgType
}

