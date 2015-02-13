#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

TARGET = ToolsSpec

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

# Common QMake directive
include($$TOP_ROOT/common/common.pri)

# Common libraries
include($$TOP_ROOT/libs/PhSync/PhSync.pri)

# Tool specific directive
include($$TOP_ROOT/specs/ToolsSpec/ToolsSpec.pri)

# Common spec directive
include($$TOP_ROOT/specs/CommonSpec.pri)

# Deployement (why in spec????)
PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
