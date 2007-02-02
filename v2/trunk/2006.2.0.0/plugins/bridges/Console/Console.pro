include( ../../plugins.inc )

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/bridges

FORMS	= src/ui/UISettingsConsole.ui

HEADERS	= src/ui/UISettingsConsole.h \
	src/ConsoleCommandParser.h \
	src/ConsoleCommand.h \
	src/Console.h

SOURCES	= src/ui/UISettingsConsole.cpp \
	src/Console.cpp
