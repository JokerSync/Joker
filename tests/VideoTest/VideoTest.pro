TEMPLATE = app
TARGET = VideoTest

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhVideo/PhVideo.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)

HEADERS += MainView.h \
    PhVideoSynchronizer.h

SOURCES += \
    main.cpp \
    MainView.cpp \
    PhVideoSynchronizer.cpp

FORMS += \
    MainView.ui

