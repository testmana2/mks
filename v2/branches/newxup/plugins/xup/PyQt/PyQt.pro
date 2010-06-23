TARGET	= PyQt
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH	*= src/gui . 

RESOURCES	= src/resources/PyQt.qrc


HEADERS	= src/PyQt.h \
	src/PyQtProjectItem.h \
	src/gui/UIPyQtEditor.h

SOURCES	= src/PyQt.cpp \
	src/PyQtProjectItem.cpp \
	src/gui/UIPyQtEditor.cpp

FORMS	=  \
	src/gui/UIPyQtEditor.ui
