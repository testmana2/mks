TEMPLATE	= app

TARGET	= myapp
DESTDIR	= bin
DLLDESTDIR	= bin/dll

CONFIG	+= qt phonon webkit thread console debug_and_release ppc x86 x11 windows rtti exceptions
QT	= core gui

HEADERS	+= test.h

SOURCES	+= test1.cpp source2.cpp \
	source3.cpp \
	"source 4.cpp" \
	source5.cpp

RESOURCES	+= resources.qrc

RC_FILE	+= windows.rc
