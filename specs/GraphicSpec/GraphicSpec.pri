#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)

SOURCES += $$TOP_ROOT/specs/GraphicSpec/GraphicSpec.cpp
SOURCES += $$TOP_ROOT/specs/GraphicSpec/GraphicTextSpec.cpp

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/specs/*.bmp) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/*) $${RESOURCES_PATH} $${CS}
