# Monkey Studio 2 project file

BUILD_PATH	 = ../build

# include qscintilla framework
include( ../qscintilla/qscintilla.pro )

unix:OBJECTS_DIR	= $${BUILD_PATH}/.o/unix
win32:OBJECTS_DIR	= $${BUILD_PATH}/.o/win32
mac:OBJECTS_DIR	= $${BUILD_PATH}/.o/mac
UI_DIR	= $${BUILD_PATH}/.ui
MOC_DIR	= $${BUILD_PATH}/.moc
RCC_DIR	= $${BUILD_PATH}/.rcc

unix:ICON	= src/resources/icons/application/monkey2.png
mac:ICON	= src/resources/icons/application/monkey2.icns
win32:ICON	= src/resources/icons/application/monkey2.ico

TEMPLATE	= app
LANGUAGE	= C++
DESTDIR	= ../bin
TARGET	= monkey

COPYRIGHTS	= "(c) 2005 - 2007 Azevedo Filipe"
DOMAIN	= "www.monkeystudio.org"
VERSION	= "1.7.0"
PROGRAM_NAME	= "Monkey Studio"

DEFINES	*= MONKEY_CORE_BUILD "PROGRAM_NAME=\"\\\"$${PROGRAM_NAME}\\\"\"" "PROGRAM_VERSION=\"\\\"$${VERSION}\\\"\"" "PROGRAM_DOMAIN=\"\\\"$${DOMAIN}\\\"\"" "PROGRAM_COPYRIGHTS=\"\\\"$${COPYRIGHTS}\\\"\""

win32:RC_FILE	*= monkey.rc
RESOURCES	*= src/resources/resources.qrc

CONFIG	*= qt warn_on debug app_bundle thread x11 windows console
win32{
#	CONFIG -= debug
#	CONFIG *= release
}
QT	*= gui core

LIBS	*= -L$${BUILD_PATH}
linux-g++:LIBS	*= -rdynamic
win32-msvc:LIBS	*= /IMPLIB:$${BUILD_PATH}/monkey.lib
win32-g++:LIBS	*= -Wl,--out-implib,$${BUILD_PATH}/libmonkey.a

INCLUDEPATH	*= . src src/maininterface src/maininterface/ui src/workspace src/recentsmanager src/qscintillamanager src/qscintillamanager/ui

# include fresh framework
include( ../fresh/fresh.pro )

FORMS	+= src/maininterface/ui/UITranslator.ui \
	src/maininterface/ui/UIAbout.ui \
	src/maininterface/ui/UIEditTemplate.ui \
	src/maininterface/ui/UIAddAbbreviation.ui \
	src/maininterface/ui/UISettings.ui \
	src/qscintillamanager/ui/pSearch.ui

HEADERS	+= src/maininterface/ui/UITranslator.h \
	src/maininterface/ui/UIAbout.h \
	src/qscintillamanager/pQScintilla.h \
	src/maininterface/ui/UIEditTemplate.h \
	src/maininterface/ui/UIAddAbbreviation.h \
	src/maininterface/ui/UISettings.h \
	src/recentsmanager/pRecentsManager.h \
	src/workspace/pAbstractChild.h \
	src/qscintillamanager/ui/pSearch.h \
	src/qscintillamanager/pEditor.h \
	src/workspace/pChild.h \
	src/workspace/UISaveFiles.h \
	src/workspace/pFileManager.h \
	src/workspace/pWorkspace.h \
	src/maininterface/UIMain.h

SOURCES	+= src/maininterface/ui/UITranslator.cpp \
	src/maininterface/ui/UIAbout.cpp \
	src/qscintillamanager/pQScintilla.cpp \
	src/maininterface/ui/UIEditTemplate.cpp \
	src/maininterface/ui/UIAddAbbreviation.cpp \
	src/maininterface/ui/UISettings.cpp \
	src/recentsmanager/pRecentsManager.cpp \
	src/qscintillamanager/ui/pSearch.cpp \
	src/qscintillamanager/pEditor.cpp \
	src/workspace/pChild.cpp \
	src/workspace/UISaveFiles.cpp \
	src/workspace/pFileManager.cpp \
	src/workspace/pWorkspace.cpp \
	src/maininterface/UIMain.cpp \
	src/main.cpp
