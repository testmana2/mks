TARGET	= PHP
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/interpreter

RESOURCES	= src/resources/PHP.qrc

HEADERS	= src/PHP.h \
	src/PHPParser.h

SOURCES	= src/PHP.cpp
