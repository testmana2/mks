TEMPLATE = lib
CONFIG *= staticlib

TARGET = Utils

DEFINES += QWORKBENCH_UTILS_LIBRARY

include(../../qworkbenchlibrary.pri)

SOURCES += pathchooser.cpp \
         basevalidatinglineedit.cpp

HEADERS +=  pathchooser.h \
         qtcassert.h \
         utils_global.h \
         basevalidatinglineedit.h
