HEADERS += \
    ../../libs/PhGraphicStrip/PhGraphicStrip.h \
    ../../libs/PhGraphicStrip/PhGraphicStripView.h \
    ../../libs/PhGraphicStrip/PhGraphicSettings.h

SOURCES += \
    ../../libs/PhGraphicStrip/PhGraphicStrip.cpp \
    ../../libs/PhGraphicStrip/PhGraphicStripView.cpp \
    ../../libs/PhGraphicStrip/PhGraphicSettings.cpp

QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/img/motif-240.png $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/fonts/SWENSON.TTF $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/fonts/ARIAL.TTF $${RESOURCES_PATH};
