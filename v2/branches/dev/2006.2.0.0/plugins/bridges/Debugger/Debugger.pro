warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

include( ../../plugins.inc )

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/bridges
RESOURCES	+= src/resources/DebuggerResources.qrc

HEADERS	= src/Debugger.h

SOURCES	= src/Debugger.cpp
