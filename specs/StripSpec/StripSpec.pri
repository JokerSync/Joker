#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)

SOURCES += $$TOP_ROOT/specs/StripSpec/StripDocSpec.cpp

win32 {
	STRIPTESTFILES = $${TOP_ROOT}/data/strip
}
else {
	STRIPTESTFILES = $${TOP_ROOT}/data/strip/
}

QMAKE_POST_LINK += $${QMAKE_COPY_DIR} $$shell_path($${STRIPTESTFILES}) $${RESOURCES_PATH} $${CS}
