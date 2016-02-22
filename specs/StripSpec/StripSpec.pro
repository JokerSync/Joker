
# Copyright (C) 2012-2015 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

TARGET = StripSpec

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

# Common spec directive
include($$TOP_ROOT/specs/CommonSpec.pri)

# Common libraries
include($$TOP_ROOT/libs/PhSync/PhSync.pri)

# Strip specific directive
include($$TOP_ROOT/specs/StripSpec/StripSpec.pri)
