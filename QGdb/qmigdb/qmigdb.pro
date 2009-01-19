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

INCLUDEPATH	= libmigdb/src

HEADERS	= libmigdb/src/mi_gdb.h \
	src/QGdbDriver.h

SOURCES	= libmigdb/src/get_free_pty.c \
	libmigdb/src/error.c \
	libmigdb/src/data_man.c \
	libmigdb/src/target_man.c \
	libmigdb/src/thread.c \
	libmigdb/src/symbol_query.c \
	libmigdb/src/var_obj.c \
	libmigdb/src/alloc.c \
	libmigdb/src/breakpoint.c \
	libmigdb/src/connect.c \
	libmigdb/src/get_free_vt.c \
	libmigdb/src/prg_control.c \
	libmigdb/src/misc.c \
	libmigdb/src/stack_man.c \
	libmigdb/src/parse.c \
	libmigdb/src/cpp_int.cc \
	src/QGdbDriver.cpp
