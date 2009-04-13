IDE_BUILD_TREE = $$OUT_PWD/../../

include(../plugins/plugins.pri)
include(../qworkbench.pri)
include(../libs/aggregation/aggregation.pri)
include(../libs/extensionsystem/extensionsystem.pri)
include(../libs/utils/utils.pri)

QT *= core gui script

TEMPLATE = app
TARGET = $$IDE_APP_TARGET
DESTDIR = ../../bin


SOURCES += main.cpp

include(../rpath.pri)
