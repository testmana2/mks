IDE_BUILD_TREE = $$OUT_PWD/../../

include(../qworkbench.pri)

TEMPLATE = app
TARGET = $$IDE_APP_TARGET
DESTDIR = ../../bin


SOURCES += main.cpp

include(../rpath.pri)
