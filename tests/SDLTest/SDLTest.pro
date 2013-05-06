QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    dwwindow.cpp \
    SDLMain.cpp

HEADERS  += dwwindow.h

FORMS    += dwwindow.ui


INCLUDEPATH += /Library/Frameworks/
LIBS += -framework SDL -framework SDL_image -framework SDL_ttf


