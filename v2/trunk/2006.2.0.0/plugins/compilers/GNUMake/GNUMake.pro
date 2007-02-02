include( ../../plugins.inc )

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
unix:INCLUDEPATH	+= ../../bridges/Console/src/
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/compilers

FORMS	= src/ui/UISettingsGNUMake.ui

HEADERS	= src/ui/UISettingsGNUMake.h \
	../../bridges/Console/src/ConsoleCommandParser.h \
	src/GNUMakeParser.h \
	src/GNUMake.h

SOURCES	= src/ui/UISettingsGNUMake.cpp \
	src/GNUMakeParser.cpp \
	src/GNUMake.cpp
