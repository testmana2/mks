include( "../../plugins.inc" )

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR$$/projects
RESOURCES	= src/resources/qmake_resources.qrc

SOURCES	= src/QMake.cpp \
	src/QMakeProjectItem.cpp \
	src/QMakeProjectItemModel.cpp \
	src/QMakeProjectProxy.cpp

HEADERS	= src/QMake.h \
	src/QMakeProjectItem.h \
	src/QMakeProjectItemModel.h \
	src/QMakeProjectProxy.h

