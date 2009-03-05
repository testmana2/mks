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

CREATOR_LIB	= Debugger
CREATOR_PATH	= ../debugger

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
	unix:LIBS	+= -l$$join(CREATOR_LIB,,,_debug)
	else:LIBS	+= -l$$join(CREATOR_LIB,,,d)
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
	LIBS	+= -l$$CREATOR_LIB
}

PRE_TARGETDEPS	= $${CREATOR_PATH}

INCLUDEPATH	= $${CREATOR_SRC_PATH}

FORMS	= src/maininterface/UIMonkeyDbg.ui

HEADERS	= src/pEditor.h \
	src/maininterface/UIMonkeyDbg.h

SOURCES	= src/main.cpp \
	src/pEditor.cpp \
	src/maininterface/UIMonkeyDbg.cpp

RESOURCES	= src/resources/monkeydbg.qrc