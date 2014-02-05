QT += serialport multimedia


mac{
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lltc -lm -lportaudio

}
win32{
}
HEADERS += \
    ../../libs/PhSync/PhSonyController.h \
    ../../libs/PhSync/PhSonyMasterController.h \
    ../../libs/PhSync/PhSonySlaveController.h \
	../../libs/PhSync/tests/PhSonyControllerTest.h \
	../../libs/PhSync/PhLtcReader.h \
	../../libs/PhSync/PhLtcWriter.h


SOURCES += \
    ../../libs/PhSync/PhSonyController.cpp \
    ../../libs/PhSync/PhSonyMasterController.cpp \
    ../../libs/PhSync/PhSonySlaveController.cpp \
    ../../libs/PhSync/tests/PhSonyControllerTest.cpp \
	../../libs/PhSync/PhLtcReader.cpp \
    ../../libs/PhSync/PhLtcWriter.cpp


