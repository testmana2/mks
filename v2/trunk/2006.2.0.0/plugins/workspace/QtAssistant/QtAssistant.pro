warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

include( "../../plugins.inc" )
include( "src/3rdparty/assistant/assistant.pro" )

TEMPLATE	= lib
INCLUDEPATH	= $$INCLUDEPATH
INCLUDEPATH	+= src/3rdparty/assistant
DEPENDPATH	+= src/3rdparty/assistant
CONFIG	+= warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/workspace
#RESOURCES	= src/3rdparty/assistant/assistant.qrc

HEADERS = $$HEADERS
HEADERS	+= src/QtAssistant.h \
	src/QtAssistantChild.h

SOURCES = $$SOURCES
SOURCES	+= src/QtAssistant.cpp \
	src/QtAssistantChild.cpp
