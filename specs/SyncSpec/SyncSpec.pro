#
# Copyright (C) 2012-2015 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

TARGET = SyncSpec

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

# Common QMake directive
include($$TOP_ROOT/common/common.pri)

# Sync specific directive
include($$TOP_ROOT/specs/SyncSpec/SyncSpec.pri)

# Common spec directive
include($$TOP_ROOT/specs/CommonSpec.pri)
