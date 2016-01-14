#
# Copyright (C) 2012-2015 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

TARGET = GraphicStripSpec

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

# Common QMake directive
include($$TOP_ROOT/common/common.pri)

# Common libraries
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)
include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)

# GraphicStrip specific directive
include($$TOP_ROOT/specs/GraphicStripSpec/GraphicStripSpec.pri)

# Common spec directive
include($$TOP_ROOT/specs/CommonSpec.pri)
