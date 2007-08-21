include( "../../plugins.inc" )

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/projects
RESOURCES	= src/resources/qmake_resources.qrc

FORMS	= src/ui/UISettingsQMake.ui \
	src/ui/UIProjectSettingsQMake.ui

HEADERS	= src/ui/UISettingsQMake.h \
	src/ui/UIProjectSettingsQMake.h \
	src/QMakeProjectItem.h \
	src/QMakeProjectItemModel.h \
	src/QMakeProjectProxy.h \
	src/QMakeProjectSettingsProxy.h \
	src/QMake.h
	

SOURCES	= src/ui/UISettingsQMake.cpp \
	src/ui/UIProjectSettingsQMake.cpp \
	src/QMakeProjectItem.cpp \
	src/QMakeProjectItemModel.cpp \
	src/QMakeProjectProxy.cpp \
	src/QMakeProjectSettingsProxy.cpp \
	src/QMake.cpp
