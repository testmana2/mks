warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

include( "../../plugins.inc" )

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
INCLUDEPATH	= ../../../monkey/src/pluginsmanager/ ../../../monkey/src/workspacemanager/ ../../../monkey/src/3rdparty/widgets/pTabWorkspace/ ../../../monkey/src/monkeyeditor/
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR$$/childs

SOURCES	= src/Text.cpp \
	src/TextChild.cpp

HEADERS	= src/Text.h \
	src/TextChild.h

