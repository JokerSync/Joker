HEADERS += \
#    ../../libs/PhCommonUI/PhLeap.h \
#	../../libs/PhCommonUI/PhMediaPanel.h \
    ../../libs/PhCommonUI/TimecodeEdit.h

SOURCES += \
#   ../../libs/PhCommonUI/PhLeap.cpp \
#	../../libs/PhCommonUI/PhMediaPanel.cpp \
    ../../libs/PhCommonUI/TimecodeEdit.cpp

FORMS +=

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
