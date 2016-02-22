#
# Copyright (C) 2012-2015 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#
# Project created by QtCreator 2014-12-03-01T17:54:58
#

TARGET = CommonUISpec

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

# Common spec directive
include($$TOP_ROOT/specs/CommonSpec.pri)

# Common libraries
include($$TOP_ROOT/libs/PhSync/PhSync.pri)

# CommonUI specific directive
include($$TOP_ROOT/specs/CommonUISpec/CommonUISpec.pri)
