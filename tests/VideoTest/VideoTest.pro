TEMPLATE = app
TARGET = VideoTest

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhVideo/PhVideo.pri)

HEADERS += MainView.h

SOURCES += \
    main.cpp \
    MainView.cpp \

