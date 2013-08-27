TEMPLATE = app
TARGET = VideoTest

QT += multimedia multimediawidgets

INCLUDEPATH += ../../libs

HEADERS = \
        MainView.h \
    ../../libs/PhVideo/PhVideoView.h \
    ../../libs/PhVideo/PhVideoController.h \
    ../../libs/PhTools/PhTimeCode.h \
    ../../libs/PhTools/PhClock.h

SOURCES = \
    main.cpp \
        MainView.cpp \
    ../../libs/PhVideo/PhVideoView.cpp \
    ../../libs/PhVideo/PhVideoController.cpp \
    ../../libs/PhTools/PhTimeCode.cpp \
    ../../libs/PhTools/PhTime.cpp \
    ../../libs/PhTools/PhClock.cpp

QT+=widgets

FORMS += \
    ../../libs/PhCommonUI/PhMediaControllerView.ui
