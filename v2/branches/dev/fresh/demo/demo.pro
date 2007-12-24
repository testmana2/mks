# fresh demo project file

TEMPLATE	= app

TARGET	= fresh_demo

BUILD_PATH	 = build

include( ../fresh.pri )

SOURCES	+= main.cpp

CONFIG += debug_and_release

LIBS += -L../../build -lfresh_debug
