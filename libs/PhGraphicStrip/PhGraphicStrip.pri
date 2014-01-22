HEADERS += \
    ../../libs/PhGraphicStrip/PhGraphicStrip.h \
    ../../libs/PhGraphicStrip/PhGraphicStripView.h

SOURCES += \
    ../../libs/PhGraphicStrip/PhGraphicStrip.cpp \
    ../../libs/PhGraphicStrip/PhGraphicStripView.cpp

mac{
	QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/img/motif-240.png $${RESOURCES_PATH};
	QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/fonts/SWENSON.TTF $${RESOURCES_PATH};
}
win32{

	RESOURCES_PATH = $$shell_path(./debug/)

	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}\data\img\motif-240.png) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}\data\fonts\SWENSON.TTF) $${RESOURCES_PATH} $${CS}

}
