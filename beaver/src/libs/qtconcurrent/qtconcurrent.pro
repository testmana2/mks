TEMPLATE = lib

include(../../qworkbenchlibrary.pri)

TARGET = QtConcurrent
DEFINES += BUILD_QTCONCURRENT

HEADERS += \
    qtconcurrent_global.h \
    multitask.h \
    runextensions.h
