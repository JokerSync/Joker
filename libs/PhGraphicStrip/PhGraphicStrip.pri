#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
    ../../libs/PhGraphicStrip/PhGraphicStrip.h \
    ../../libs/PhGraphicStrip/PhGraphicStripView.h \
    ../../libs/PhGraphicStrip/PhGraphicStripSettings.h

SOURCES += \
    ../../libs/PhGraphicStrip/PhGraphicStrip.cpp \
    ../../libs/PhGraphicStrip/PhGraphicStripView.cpp

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/SWENSON.TTF) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/ARIAL.TTF) $${RESOURCES_PATH} $${CS}

