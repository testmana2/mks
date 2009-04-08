TEMPLATE = lib
TARGET = TextEditor
DEFINES += TEXTEDITOR_LIBRARY
include(../../qworkbenchplugin.pri)
include(texteditor_dependencies.pri)
SOURCES += \
    basetexteditor.cpp \
    basetextmark.cpp
HEADERS += \
    basetexteditor.h \
    basetextmark.h \
    itexteditor.h \
    texteditorconstants.h
