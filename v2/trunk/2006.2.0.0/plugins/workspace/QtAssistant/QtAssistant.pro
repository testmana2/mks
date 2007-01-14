warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

include( ../../plugins.inc )
include( src/3rdparty/assistant/assistant.pro )

win32 {
	RC_FILE	-= assistant.rc
}

mac {
	ICON	-= assitant.icns
}

contains( SOURCES, main.cpp ) {
	SOURCES	-= main.cpp
}

DEPENDPATH	= src/3rdparty/assistant
TEMPLATE	= lib
LANGUAGE	= C++
INCLUDEPATH	= $$INCLUDEPATH src/3rdparty/assistant/
CONFIG	+= warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/workspace
TARGET	= QtAssistant

HEADERS	= $$HEADERS \
	src/QtAssistant.h \
	src/QtAssistantChild.h

SOURCES	= $$SOURCES \
	src/QtAssistant.cpp \
	src/QtAssistantChild.cpp

OPENEDFILES	= src/QtAssistant.h \
	src/QtAssistantChild.h