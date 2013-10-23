TEMPLATE = app
TARGET = VideoSyncTest

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhSync/PhSync.pri)
include($${JOKER_ROOT}/libs/PhGraphic/PhGraphic.pri)
include($${JOKER_ROOT}/libs/PhVideo/PhVideo.pri)
include($${JOKER_ROOT}/libs/PhCommonUI/PhCommonUI.pri)

HEADERS += MainView.h

SOURCES += \
    main.cpp \
    MainView.cpp \

FORMS += \
    MainView.ui

