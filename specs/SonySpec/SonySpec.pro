#
# Copyright (C) 2012-2016 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

TARGET = SonySpec

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

# Common QMake directive
include($$TOP_ROOT/common/common.pri)

# Common libraries
include($$TOP_ROOT/libs/PhSync/PhSync.pri)

# Sony specific directive
include($$TOP_ROOT/specs/SonySpec/SonySpec.pri)

# Common spec directive
include($$TOP_ROOT/specs/CommonSpec.pri)
