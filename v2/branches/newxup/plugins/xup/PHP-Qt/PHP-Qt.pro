TARGET	= PHP-Qt
include( ../../plugins.pri )
DESTDIR	= $$MONKEY_PLUGINS_DIR/xup

INCLUDEPATH	*= src/gui . 

RESOURCES	= src/resources/PHP-Qt.qrc

FORMS	=  \
	src/gui/UIPHPEditor.ui \

HEADERS	= src/PHPQt.h \
	src/PHPQtProjectItem.h \
	src/gui/UIPHPEditor.h

SOURCES	= src/PHPQt.cpp \
	src/PHPQtProjectItem.cpp \
	src/gui/UIPHPEditor.cpp
