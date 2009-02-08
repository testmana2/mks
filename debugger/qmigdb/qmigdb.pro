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

MIGDB_LIB	= migdb
MIGDB_PATH	= ../libmigdb
MIGDB_SRC_PATH	= $${MIGDB_PATH}/src

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
	unix:LIBS	+= -l$$join(MIGDB_LIB,,,_debug)
	else:LIBS	+= -l$$join(MIGDB_LIB,,,d)
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
	LIBS	+= -l$${MIGDB_LIB}
}

PRE_TARGETDEPS	= $${MIGDB_PATH}

INCLUDEPATH	= $${MIGDB_SRC_PATH} \
	src/driver \
	src/widgets \
	src/structures

HEADERS	= src/driver/QGdbDriver.h \
	src/widgets/CallStackWidget.h \
	src/widgets/BreakpointWidget.h \
	src/structures/QGdbBreakpoint.h \
	src/structures/QGdb.h \
	src/structures/QGdbSignal.h \
	src/structures/QGdbCallStack.h \
	src/driver/QGdb-Driver.h \
	src/widgets/BreakpointEditorWidget.h

SOURCES	= src/driver/QGdbDriver.cpp \
	src/widgets/CallStackWidget.cpp \
	src/widgets/BreakpointWidget.cpp \
	src/structures/QGdbBreakpoint.cpp \
	src/structures/QGdb.cpp \
	src/structures/QGdbSignal.cpp \
	src/structures/QGdbCallStack.cpp \
	src/driver/QGdb-Driver.cpp \
	src/widgets/BreakpointEditorWidget.cpp
