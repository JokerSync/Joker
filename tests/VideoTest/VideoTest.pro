TEMPLATE = app
TARGET = VideoTest

QT += multimedia multimediawidgets

INCLUDEPATH += ../../libs

HEADERS = \
	PhVideo.h \
    ../../libs/PhVideo/PhVideoView.h

SOURCES = \
    main.cpp \
	PhVideo.cpp \
    ../../libs/PhVideo/PhVideoView.cpp

QT+=widgets
