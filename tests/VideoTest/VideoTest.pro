TEMPLATE = app
TARGET = VideoTest

QT += multimedia multimediawidgets

INCLUDEPATH += ../../libs

HEADERS = \
        MainView.h \
    ../../libs/PhVideo/PhVideoView.h \
    ../../libs/PhVideo/PhVideoController.h

SOURCES = \
    main.cpp \
        MainView.cpp \
    ../../libs/PhVideo/PhVideoView.cpp \
    ../../libs/PhVideo/PhVideoController.cpp

QT+=widgets
