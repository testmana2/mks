###########################################################################################
##		Created using Monkey Studio v1.8.3.0
##
##	Author    : MkS Team <contact@monkeystudio.org>
##	Project   : migdb
##	FileName  : migdb.pro
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
TARGET	= $$quote(migdb)
CONFIG	+= staticlib debug_and_release
QT	-= gui core
DESTDIR	= ../build/
BUILD_PATH	= ../build/libmigdb

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

INCLUDEPATH	= src

HEADERS	= src/mi_gdb.h

SOURCES	= src/get_free_pty.c \
	src/error.c \
	src/data_man.c \
	src/target_man.c \
	src/thread.c \
	src/symbol_query.c \
	src/var_obj.c \
	src/alloc.c \
	src/breakpoint.c \
	src/connect.c \
	src/get_free_vt.c \
	src/prg_control.c \
	src/misc.c \
	src/stack_man.c \
	src/parse.c \
	src/cpp_int.cc
