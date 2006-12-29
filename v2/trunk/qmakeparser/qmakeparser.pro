warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

unix:OBJECTS_DIR	= build/.o/unix
win32:OBJECTS_DIR	= build/.o/win32
mac:OBJECTS_DIR	= build/.o/mac
UI_DIR	= build/.ui
MOC_DIR	= build/.moc
RCC_DIR	= build/.rcc

CODEC	= MOUARF # merde de commentaire !

TEMPLATE	= app
QT	= gui core
LANGUAGE	= C++
INCLUDEPATH	= . src src/ui
CONFIG	+= qt warn_on debug app_bundle thread x11 windows
RESOURCES	= src/resources/qmakeproject_resources.qrc

SOURCES	= src/main.cpp \
	src/AbstractProjectItemModel.cpp \
	src/QMakeProjectItemModel.cpp \
	src/QMakeProjectItem.cpp \
	src/QMakeProjectProxy.cpp \
	src/AbstractProjectProxy.cpp \
	src/ui/UIProjectsManager.cpp

FORMS	= src/ui/UIProjectsManager.ui

OPENEDFILES	= src/AbstractProjectProxy.h \
	src/AbstractProjectItemModel.h \
	src/AbstractProjectItemModel.cpp \
	src/QMakeProjectProxy.h \
	src/QMakeProjectProxy.cpp \
	src/QMakeProjectItemModel.h \
	src/QMakeProjectItemModel.cpp \
	src/QMakeProjectItem.h \
	src/QMakeProjectItem.cpp \
	src/AbstractProjectProxy.cpp \
	src/ui/UIProjectsManager.h \
	src/ui/UIProjectsManager.cpp \
	ToDo.txt

HEADERS	= src/AbstractProjectItemModel.h \
	src/QMakeProjectItemModel.h \
	src/QMakeProjectItem.h \
	src/AbstractProjectProxy.h \
	src/QMakeProjectProxy.h \
	src/ui/UIProjectsManager.h

