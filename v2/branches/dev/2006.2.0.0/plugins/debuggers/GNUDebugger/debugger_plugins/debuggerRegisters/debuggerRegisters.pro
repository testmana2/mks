warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

unix:OBJECTS_DIR	= build/.o/unix
win32:OBJECTS_DIR	= build/.o/win32
mac:OBJECTS_DIR	= build/.o/mac
UI_DIR	= build/.ui
MOC_DIR	= build/.moc
RCC_DIR	= build/.rcc

TEMPLATE	= lib
QT	= gui core
LANGUAGE	= C++
INCLUDEPATH	= ../../src

CONFIG	+= qt warn_on release lib_bundle thread plugin
DESTDIR	= build/
TARGET	= debuggerRegisters

SOURCES	= debuggerRegisters.cpp \
	settingRegister.cpp

FORMS	= settingRegister.ui

HEADERS	= debuggerRegisters.h \
	settingRegister.h

