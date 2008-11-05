TEMPLATE	= app

TARGET	= myapp
DESTDIR	= bin
DLLDESTDIR	= bin/dll

CONFIG	+= qt phonon webkit thread console debug_and_release ppc x86 x11 windows rtti exceptions
QT	= core gui

HEADERS	+= test.h

SOURCES	+= test6.cpp \
	test2.cpp \
	test5.cpp \
	test1.cpp \
	test4.cpp \
	test3.cpp

SOURCES += toto.cpp

RESOURCES	+= resources.qrc

RC_FILE	+= windows.rc
