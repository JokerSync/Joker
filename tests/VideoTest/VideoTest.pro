TEMPLATE = app
TARGET = VideoTest

QT += multimedia multimediawidgets

INCLUDEPATH += ../../libs

HEADERS = \
        MainView.h \
    ../../libs/PhVideo/PhVideoView.h

SOURCES = \
    main.cpp \
        MainView.cpp \
    ../../libs/PhVideo/PhVideoView.cpp

QT+=widgets
