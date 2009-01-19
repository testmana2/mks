###########################################################################################
##		Created using Monkey Studio v1.8.3.0svn_release
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
	EXECUTE_DEBUG	= monkeydebugger_debug
	QT_VERSION	= Qt System (4.4.3)
}

TEMPLATE	= app
LANGUAGE	= C++/Qt4
TARGET	= monkeydebugger
CONFIG	+= debug_and_release
BUILD_PATH	= ./build

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
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
}

SOURCES	= src/main.cpp \
	src/MainWindow.cpp \
	src/GnuDebugger.cpp \
	src/FileManager.cpp \
	src/CallStackWidget.cpp

FORMS	= src/MainWindow.ui
HEADERS	= src/MainWindow.h \
	src/GnuDebugger.h \
	src/FileManager.h \
	src/CallStackWidget.h

INCLUDEPATH	= libmigdb/src

LIBS	= -Llibmigdb/src -lmigdb -lqscintilla2
