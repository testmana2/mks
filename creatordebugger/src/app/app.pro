IDE_BUILD_TREE = $$OUT_PWD/../../

include(../plugins/plugins.pri)
include(../qworkbench.pri)
include(../libs/aggregation/aggregation.pri)
include(../libs/extensionsystem/extensionsystem.pri)
include(../libs/cplusplus/cplusplus.pri)
include(../libs/utils/utils.pri)

INCLUDEPATH *= ../libs/extensionsystem ../plugins/coreplugin

QT *= core gui script

TEMPLATE = app
TARGET = $$IDE_APP_TARGET
DESTDIR = ../../bin

CONFIG -= release
CONFIG *= debug

SOURCES += main.cpp

include(../rpath.pri)