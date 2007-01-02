include( "../../plugins.inc" )

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR$$/childs

SOURCES	= src/Text.cpp \
	src/TextChild.cpp

HEADERS	= src/Text.h \
	src/TextChild.h

