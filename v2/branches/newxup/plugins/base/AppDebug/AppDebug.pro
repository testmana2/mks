###########################################################################################
##		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
##
##	Author      : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
##	Project     : AppDebug
##	FileName    : AppDebug.pro
##	Date        : 2010-06-16T22:20:55
##	Description : A plugin that help to debug Mks itself
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

TARGET	= AppDebug

include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
INCLUDEPATH	*= src
RESOURCES	= src/resources/AppDebug.qrc

HEADERS	= src/AppDebug.h \
	src/DebugDockWidget.h

SOURCES	= src/AppDebug.cpp \
	src/DebugDockWidget.cpp