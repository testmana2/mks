warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

unix:OBJECTS_DIR	= build/.o/unix
win32-x-g++:OBJECTS_DIR	= build/.o/win32
win32:OBJECTS_DIR	= build/.o/win32
mac:OBJECTS_DIR	= build/.o/mac
UI_DIR	= build/.ui
MOC_DIR	= build/.moc
RCC_DIR	= build/.rcc

TEMPLATE	= app
QT	= gui core
LANGUAGE	= C++
INCLUDEPATH	= . src src/item/ src/model/ src/settings/ src/ui/
CONFIG	+= qt warn_on debug app_bundle thread x11 windows console
DESTDIR	= ./
RESOURCES	= src/resources/resources.qrc src/resources/resources2.qrc

SOURCES	= src/item/QMakeProjectItem.cpp \
	src/item/QMakeProjectItemPrivate.cpp \
	src/model/QMakeProjectModel.cpp \
	src/settings/QMakeProjectItemDelegate.cpp \
	src/ui/UIItemSettings.cpp \
	src/ui/UIProjectsViewer.cpp \
	src/ui/UIQMakeProjectSettings.cpp \
	src/main.cpp \
	src/model/QMakeProjectParser.cpp \
	src/model/QMakeProjectProxy.cpp

FORMS	= src/ui/UIItemSettings.ui \
	src/ui/UIProjectsViewer.ui \
	src/ui/UIQMakeProjectSettings.ui

OPENEDFILES	= src/item/QMakeProjectItem.h \
	src/item/QMakeProjectItem.cpp \
	src/model/QMakeProjectModel.h \
	src/model/QMakeProjectModel.cpp \
	src/ui/UIProjectsViewer.h \
	src/ui/UIProjectsViewer.cpp \
	src/ui/UIQMakeProjectSettings.h \
	src/ui/UIQMakeProjectSettings.cpp \
	src/model/QMakeProjectProxy.h \
	src/model/QMakeProjectProxy.cpp \
	src/ui/UIItemSettings.h \
	src/ui/UIItemSettings.cpp

HEADERS	= src/item/QMakeProjectItem.h \
	src/item/QMakeProjectItemPrivate.h \
	src/model/QMakeProjectModel.h \
	src/settings/QMakeProjectItemDelegate.h \
	src/ui/UIItemSettings.h \
	src/ui/UIProjectsViewer.h \
	src/ui/UIQMakeProjectSettings.h \
	src/model/QMakeProjectParser.h \
	src/model/QMakeProjectProxy.h

