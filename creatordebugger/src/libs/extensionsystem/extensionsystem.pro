TEMPLATE = lib
CONFIG *= staticlib

TARGET = ExtensionSystem
QT += xml
DEFINES += EXTENSIONSYSTEM_LIBRARY
include(../../qworkbenchlibrary.pri)
include(extensionsystem_dependencies.pri)

HEADERS += \
    iplugin.h \
    extensionsystem_global.h \
    pluginmanager.h
SOURCES += \
    iplugin.cpp
