###########################################################################################
##		Created using Monkey Studio v1.8.3.0
##
##	Author    : MkS Team <contact@monkeystudio.org>
##	Project   : qmigdb
##	FileName  : qmigdb.pro
##	Date      : 2009-01-19T20:40:25
##	License   : GPL
##	Comment   : Creating using Monkey Studio IDE
##	Home Page   : http://monkeystudio.org
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

XUPProjectSettings {
	EDITOR	= QMake
}

TEMPLATE	= lib
LANGUAGE	= C++/Qt4
TARGET	= $$quote(qmigdb)
CONFIG	+= staticlib debug_and_release
DESTDIR	= ../build
BUILD_PATH	= ../build/qmigdb

LIBMIGDB_SRC_PATH = ../libmigdb/src

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

INCLUDEPATH	= src/driver \
		  src/widgets \
		  $${LIBMIGDB_SRC_PATH}

HEADERS	=  src/driver/QGdbDriver.h \
	\
	src/widgets/CallStackWidget.h

SOURCES	= src/driver/QGdbDriver.cpp \
	\
	src/widgets/CallStackWidget.cpp
