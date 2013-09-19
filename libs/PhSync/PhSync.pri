QT += serialport

HEADERS += \
    ../../libs/PhSync/PhSonyController.h \
    ../../libs/PhSync/PhSonyMasterController.h \
    ../../libs/PhSync/PhSonySlaveController.h \
    ../../libs/PhSync/tests/PhSonyControllerTest.h \
    ../../libs/PhSync/PhClockSynchronizer.h

SOURCES += \
    ../../libs/PhSync/PhSonyController.cpp \
    ../../libs/PhSync/PhSonyMasterController.cpp \
    ../../libs/PhSync/PhSonySlaveController.cpp \
    ../../libs/PhSync/tests/PhSonyControllerTest.cpp \
    ../../libs/PhSync/PhClockSynchronizer.cpp

INCLUDEPATH += ../../libs/PhSync
INCLUDEPATH += ../../libs/PhTools
