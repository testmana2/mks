warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

include( ../../plugins.inc )

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
unix:INCLUDEPATH	+= ../../bridges/Console/src/
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/compilers

SOURCES	= src/GNUMake.cpp \
	src/ui/UISettings.cpp

FORMS	= src/ui/UISettings.ui

OPENEDFILES	= src/GNUMake.h \
	src/GNUMake.cpp

HEADERS	= src/GNUMake.h \
	src/ui/UISettings.h

