TARGET	= XUP
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH	*= src

HEADERS	=  \
	src/XUP.h \
	src/gui/UIXUPEditor.h

SOURCES	=  \
	src/XUP.cpp \
	src/gui/UIXUPEditor.cpp
