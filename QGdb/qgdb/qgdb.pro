###########################################################################################
##		Created using Monkey Studio v1.8.3.0
##
##	Author    : Andrei Kopats <hlamer at tut by>
##	Project   : Monkey Debugger
##	FileName  : Monkey Debugger.pro
##	Date      : 2009-01-13T12:46:18
##	License   : GPL
##	Comment   : Creating using Monkey Studio RAD
##	Home Page   : www.monkeystudio.org
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

XUPProjectSettings {
	EDITOR	= QMake
}

TEMPLATE	= app
LANGUAGE	= C++/Qt4
TARGET	= QGdb
CONFIG	+= debug_and_release
LIBS	+= -lqscintilla2 -L../build
DESTDIR	= ../bin
BUILD_PATH	= ../build/qgdb
QMIGDB_LIB	= qmigdb

CONFIG(debug, debug|release) {
	#Debug
	CONFIG	+= console
	unix:TARGET	= $$join(TARGET,,,_debug)
	else:TARGET	= $$join(TARGET,,,d)
	unix:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/mac
	UI_DIR	= $${BUILD_PATH}/debug/.ui
	MOC_DIR	= $${BUILD_PATH}/debug/.moc
	RCC_DIR	= $${BUILD_PATH}/debug/.rcc
	unix:LIBS	+= -l$$join(QMIGDB_LIB,,,_debug)
	else:LIBS	+= -l$$join(QMIGDB_LIB,,,d)
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
	LIBS	+= -l$$QMIGDB_LIB
}

INCLUDEPATH	= ../qmigdb/libmigdb/src \
	../qmigdb/src

FORMS	= src/MainWindow.ui

HEADERS	= src/FileManager.h \
	src/CallStackWidget.h \
	src/MainWindow.h

SOURCES	= src/MainWindow.cpp \
	src/FileManager.cpp \
	src/CallStackWidget.cpp \
	src/main.cpp