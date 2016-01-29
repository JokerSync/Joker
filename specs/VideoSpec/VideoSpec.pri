#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

include($$TOP_ROOT/libs/PhVideo/PhVideo.pri)

SOURCES += $$TOP_ROOT/specs/VideoSpec/VideoSpec.cpp

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/video/*.*) . $${CS}
