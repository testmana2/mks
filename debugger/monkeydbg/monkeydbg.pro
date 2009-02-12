###########################################################################################
##		Created using Monkey Studio v1.8.3.0
##
##	Author    : Andrei Kopats <hlamer at tut by>
##	Project   : Monkey Debugger
##	FileName  : monkeydbg.pro
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
TARGET	= monkeydbg
CONFIG	+= debug_and_release
LIBS	= -lqscintilla2 -L../build
DESTDIR	= ../bin
BUILD_PATH	= ../build/monkeydbg

MIGDB_LIB	= migdb
MIGDB_PATH	= ../libmigdb
MIGDB_SRC_PATH	= $${MIGDB_PATH}/src

QMIGDB_LIB	= qmigdb
QMIGDB_PATH	= ../qmigdb
QMIGDB_SRC_PATH	= $${QMIGDB_PATH}/src

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
	LIBS	+= -l$$QMIGDB_LIB
	LIBS	+= -l$$MIGDB_LIB
}

PRE_TARGETDEPS	= $${MIGDB_PATH} $${QMIGDB_PATH}

INCLUDEPATH	= $${MIGDB_SRC_PATH} \
	$${QMIGDB_SRC_PATH}/driver \
	$${QMIGDB_SRC_PATH}/widgets \
	$${QMIGDB_SRC_PATH}/structures

FORMS	= src/maininterface/UIMonkeyDbg.ui

HEADERS	= src/pEditor.h \
	src/maininterface/UIMonkeyDbg.h

SOURCES	= src/main.cpp \
	src/pEditor.cpp \
	src/maininterface/UIMonkeyDbg.cpp

RESOURCES	= src/resources/monkeydbg.qrc