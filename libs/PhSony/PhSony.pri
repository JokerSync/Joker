#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT += serialport

HEADERS += \
	$$TOP_ROOT/libs/PhSony/PhSonyController.h \
	$$TOP_ROOT/libs/PhSony/PhSonyMasterController.h \
	$$TOP_ROOT/libs/PhSony/PhSonySlaveController.h \
	$$TOP_ROOT/libs/PhSony/PhSonySettings.h

SOURCES += \
	$$TOP_ROOT/libs/PhSony/PhSonyController.cpp \
	$$TOP_ROOT/libs/PhSony/PhSonyMasterController.cpp \
	$$TOP_ROOT/libs/PhSony/PhSonySlaveController.cpp
