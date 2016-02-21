#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)

SOURCES += $$TOP_ROOT/specs/StripSpec/StripDocSpec.cpp

QMAKE_POST_LINK += $${QMAKE_COPY} -r $$shell_path($${TOP_ROOT}/data/strip/) $${RESOURCES_PATH} $${CS}
