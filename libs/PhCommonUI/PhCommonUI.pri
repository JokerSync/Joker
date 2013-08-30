HEADERS += \
#    ../../libs/PhCommonUI/PhLeap.h \
	../../libs/PhCommonUI/PhMediaControllerView.h

SOURCES += \
#   ../../libs/PhCommonUI/PhLeap.cpp \
	../../libs/PhCommonUI/PhMediaControllerView.cpp\

FORMS += ../../libs/PhCommonUI/PhMediaControllerView.ui

#INCLUDEPATH += $$(LEAP_SDK)/include

# Windows specific
win32 {
}

# Ubuntu specific
linux {
#LIBS += -L$$(LEAP_SDK)/lib/x64 -lLeap
}


# MacOS specific
mac {
#LIBS += -L$$(LEAP_SDK)/lib -lLeap
}

#	leap.commands += cp -r $$(LEAP_SDK)/lib/libLeap.dylib $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/ ;
