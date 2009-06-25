TEMPLATE = lib

include(../../qworkbenchlibrary.pri)

TARGET = Aggregation

DEFINES += AGGREGATION_LIBRARY

HEADERS = aggregate.h \
    aggregation_global.h

SOURCES = aggregate.cpp

