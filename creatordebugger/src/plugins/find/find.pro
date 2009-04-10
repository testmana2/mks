TEMPLATE = lib
CONFIG *= staticlib

TARGET = Find
include(../../qworkbenchplugin.pri)
include(find_dependencies.pri)
DEFINES += FIND_LIBRARY
HEADERS += \
    basetextfind.h
SOURCES += \
    basetextfind.cpp
