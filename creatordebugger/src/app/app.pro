IDE_BUILD_TREE = $$OUT_PWD/../../

include(../qworkbench.pri)
include(../libs/aggregation/aggregation.pri)
include(../libs/extensionsystem/extensionsystem.pri)
include(../libs/utils/utils.pri)
include(../plugins/plugins.pri)

TEMPLATE = app
TARGET = $$IDE_APP_TARGET
DESTDIR = ../../bin


SOURCES += main.cpp

include(../rpath.pri)
